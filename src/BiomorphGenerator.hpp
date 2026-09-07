#ifndef BIOMORPH_GENERATOR_HPP
#define BIOMORPH_GENERATOR_HPP

#include "BiomorphParameters.hpp"
#include "BiomorphImage.hpp"

#include <QImage>
#include <QString>

class BiomorphGenerator
{
public:
    BiomorphGenerator();
    ~BiomorphGenerator();

    BiomorphImage generate(const BiomorphParameters& params);

private:
    void initializeOpenCL();
    void cleanupOpenCL();

    void checkError(int error, const char* operation);

private:
    struct OpenCLState;
    OpenCLState* m_cl = nullptr;
};

#endif