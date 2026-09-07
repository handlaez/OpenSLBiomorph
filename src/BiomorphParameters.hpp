#ifndef BIOMORPH_PARAMETERS_HPP
#define BIOMORPH_PARAMETERS_HPP

struct BiomorphParameters
{
    int width = 512;
    int height = 512;

    double xmin = -1.0;
    double xmax = 1.0;
    double ymin = -1.0;
    double ymax = 1.0;

    double alpha = 1.0;
    double beta = 0.0;
    double R = 10.0;
    double freq = 0.2;

    double gc1x = 0.7;
    double gc1y = -0.35;

    double gc2x = 0.0;
    double gc2y = 1.5;

    int K = 15;
    int useSwitch = 1;
};

#endif