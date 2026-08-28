#include <iostream>
#include <vector>
#include <fstream>
#include <CL/cl.h>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Konfiguracja
const int WIDTH = 4000;
const int HEIGHT = 4000;
const double xmin = -1.0, xmax = 1.0, ymin = -1.0, ymax = 1.0;
const double alpha = 1.0, beta = 0.0, R = 10.0, freq = 0.2;
const int K = 15;
const int useSwitch = 1;

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    size_t image_size = WIDTH * HEIGHT * 4;
    std::vector<unsigned char> h_image(image_size);

    // --- OpenCL Setup ---
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, NULL, NULL);

    // Load Kernel Source (kernel.cl)
    std::ifstream k_file("kernel.cl");
    std::string k_src((std::istreambuf_iterator<char>(k_file)), std::istreambuf_iterator<char>());
    const char* srcptr = k_src.c_str();
    cl_program program = clCreateProgramWithSource(context, 1, &srcptr, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    cl_kernel kernel = clCreateKernel(program, "generate_biomorph", NULL);
    cl_mem d_image = clCreateBuffer(context, CL_MEM_WRITE_ONLY, image_size, NULL, NULL);

    // Set Arguments
    cl_double2 gc1 = { 0.7, -0.35 }, gc2 = { 0.0, 1.5 };
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_image);
    clSetKernelArg(kernel, 1, sizeof(int), &WIDTH);
    clSetKernelArg(kernel, 2, sizeof(int), &HEIGHT);
    clSetKernelArg(kernel, 3, sizeof(double), &xmin);
    clSetKernelArg(kernel, 4, sizeof(double), &xmax);
    clSetKernelArg(kernel, 5, sizeof(double), &ymin);
    clSetKernelArg(kernel, 6, sizeof(double), &ymax);
    clSetKernelArg(kernel, 7, sizeof(double), &alpha);
    clSetKernelArg(kernel, 8, sizeof(double), &beta);
    clSetKernelArg(kernel, 9, sizeof(cl_double2), &gc1);
    clSetKernelArg(kernel, 10, sizeof(cl_double2), &gc2);
    clSetKernelArg(kernel, 11, sizeof(int), &K);
    clSetKernelArg(kernel, 12, sizeof(double), &R);
    clSetKernelArg(kernel, 13, sizeof(double), &freq);
    clSetKernelArg(kernel, 14, sizeof(int), &useSwitch);

    // Exec
    size_t global_ws[2] = { (size_t)WIDTH, (size_t)HEIGHT };
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_ws, NULL, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, d_image, CL_TRUE, 0, image_size, h_image.data(), 0, NULL, NULL);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    stbi_write_png("biomorph_gpu.png", WIDTH, HEIGHT, 4, h_image.data(), WIDTH * 4);

    // Cleanup
    clReleaseMemObject(d_image);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    std::cout << "Calkowity czas (OpenSL): " << elapsed.count() << " s" << std::endl;

    return 0;
}