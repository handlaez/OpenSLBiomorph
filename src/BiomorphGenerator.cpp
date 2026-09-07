#include "BiomorphGenerator.hpp"

#include <CL/cl.h>

#include <QDebug>
#include <QImage>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

struct BiomorphGenerator::OpenCLState
{
    cl_platform_id platform = nullptr;
    cl_device_id device = nullptr;
    cl_context context = nullptr;
    cl_command_queue queue = nullptr;
    cl_program program = nullptr;
    cl_kernel kernel = nullptr;
};

BiomorphGenerator::BiomorphGenerator()
{
    initializeOpenCL();
}

BiomorphGenerator::~BiomorphGenerator()
{
    cleanupOpenCL();
}

BiomorphImage BiomorphGenerator::generate(const BiomorphParameters& params)
{
    BiomorphImage result;
    result.width = params.width;
    result.height = params.height;

    const size_t imageSize = static_cast<size_t>(params.width) * static_cast<size_t>(params.height) * 4;

    std::vector<unsigned char> pixels(imageSize);

    cl_int err;

    cl_mem d_image = clCreateBuffer(cl_->context, CL_MEM_WRITE_ONLY, imageSize, nullptr, &err);
    checkError(err, "clCreateBuffer");

    cl_double2 gc1 = { params.gc1x, params.gc1y };
    cl_double2 gc2 = { params.gc2x, params.gc2y };

    // The kernel arguments
    checkError(clSetKernelArg(cl_->kernel, 0, sizeof(cl_mem), &d_image), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 1, sizeof(int), &params.width), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 2, sizeof(int), &params.height), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 3, sizeof(double), &params.xmin), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 4, sizeof(double), &params.xmax), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 5, sizeof(double), &params.ymin), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 6, sizeof(double), &params.ymax), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 7, sizeof(double), &params.alpha), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 8, sizeof(double), &params.beta), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 9, sizeof(cl_double2), &gc1), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 10, sizeof(cl_double2), &gc2), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 11, sizeof(int), &params.K), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 12, sizeof(double), &params.R), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 13, sizeof(double), &params.freq), "clSetKernelArg");
    checkError(clSetKernelArg(cl_->kernel, 14, sizeof(int), &params.useSwitch), "clSetKernelArg");

    size_t globalWorkSize[2] = {
        static_cast<size_t>(params.width),
        static_cast<size_t>(params.height)
    };

    checkError(
        clEnqueueNDRangeKernel(cl_->queue, cl_->kernel, 2, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr),
        "clEnqueueNDRangeKernel");

    checkError(
        clEnqueueReadBuffer(cl_->queue, d_image, CL_TRUE, 0, imageSize, result.pixels.data(), 0, nullptr, nullptr),
        "clEnqueueReadBuffer");

    clReleaseMemObject(d_image);

    return result;
}

void BiomorphGenerator::checkError(int error, const char* operation)
{
    if (error != CL_SUCCESS)
    {
        throw std::runtime_error(std::string(operation) + " failed. OpenCL error: " + std::to_string(error));
    }
}

void BiomorphGenerator::initializeOpenCL()
{
    cl_ = new OpenCLState;
    cl_int err;

    checkError(clGetPlatformIDs(1, &cl_->platform, nullptr), "clGetPlatformIDs");
    checkError(clGetDeviceIDs(cl_->platform, CL_DEVICE_TYPE_GPU, 1, &cl_->device, nullptr), "clGetDeviceIDs");

    cl_->context = clCreateContext(nullptr, 1, &cl_->device, nullptr, nullptr, &err);
    checkError(err, "clCreateContext");

    cl_->queue = clCreateCommandQueueWithProperties(cl_->context, cl_->device, nullptr, &err);
    checkError(err, "clCreateCommandQueueWithProperties");

    std::ifstream file("kernel.cl");

    if (!file)
        throw std::runtime_error("Could not open kernel.cl");

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    const char* sourcePtr = source.c_str();

    cl_->program = clCreateProgramWithSource(cl_->context, 1, &sourcePtr, nullptr, &err);

    checkError(err, "clCreateProgramWithSource");
    err = clBuildProgram(cl_->program, 1, &cl_->device, nullptr, nullptr, nullptr);

    if (err != CL_SUCCESS)
    {
        size_t logSize = 0;

        clGetProgramBuildInfo(cl_->program, cl_->device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);

        std::vector<char> log(logSize);

        clGetProgramBuildInfo(cl_->program, cl_->device, CL_PROGRAM_BUILD_LOG, logSize, log.data(), nullptr);

        throw std::runtime_error("OpenCL build failed:\n" + std::string(log.data()));
    }

    cl_->kernel = clCreateKernel(cl_->program, "generate_biomorph", &err);
    checkError(err, "clCreateKernel");
}

void BiomorphGenerator::cleanupOpenCL()
{
    if (!cl_)
        return;

    if (cl_->kernel)
        clReleaseKernel(cl_->kernel);

    if (cl_->program)
        clReleaseProgram(cl_->program);

    if (cl_->queue)
        clReleaseCommandQueue(cl_->queue);

    if (cl_->context)
        clReleaseContext(cl_->context);

    delete cl_;
    cl_ = nullptr;
}