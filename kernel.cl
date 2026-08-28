#pragma OPENCL EXTENSION cl_khr_fp64 : enable

// helpers
double2 complex_mul(double2 a, double2 b) {
    return (double2)(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

double2 complex_pow3(double2 z) {
    double x2 = z.x * z.x;
    double y2 = z.y * z.y;
    return (double2)(z.x * (x2 - 3.0 * y2), z.y * (3.0 * x2 - y2));
}

double2 complex_inv_pow(double2 c, int p) {
    double r = length(c);
    double theta = atan2(c.y, c.x);
    double new_r = pow(r, (double)-p);
    double new_theta = -p * theta;
    return (double2)(new_r * cos(new_theta), new_r * sin(new_theta));
}

__kernel void generate_biomorph(
    __global uchar4* image,
    int width, int height,
    double xmin, double xmax, double ymin, double ymax,
    double alpha, double beta,
    double2 c1, double2 c2,
    int K, double R, double freq, int useSwitch) 
{
    int px = get_global_id(0);
    int py = get_global_id(1);

    if (px >= width || py >= height) return;

    double x = xmin + (xmax - xmin) * px / (width - 1);
    double y = ymin + (ymax - ymin) * py / (height - 1);

    double2 z = (double2)(x, y);
    double2 d = c1;
    int iter = 0;

    for (int i = 1; i <= K; i++) {
        // v = beta * (f(z) + d) + (1.0 - beta) * z
        double2 fz = complex_pow3(z);
        double2 v = beta * (fz + d) + (1.0 - beta) * z;
        
        // z = alpha * (f(v) + d) + (1.0 - alpha) * z
        double2 fv = complex_pow3(v);
        z = alpha * (fv + d) + (1.0 - alpha) * z;

        if (length(z) > R) {
            iter = i;
            break;
        }

        if (useSwitch) {
            if (i % 2 == 1) d = complex_inv_pow(c1, i) - d;
            else           d = complex_inv_pow(c2, i) - d;
        }
    }

    int idx = (height - 1 - py) * width + px;
    uchar4 pixel;

    if (iter == 0) {
        pixel = (uchar4)(255, 255, 255, 255);
    } else if (fabs(z.x) < R || fabs(z.y) < R) {
        double log_zn = log(length(z));
        double nu = log(log_zn / log(R)) / log(3.0);
        double t = iter + 1.0 - nu;
        if (t < 0) t = 0;

        double r_val = sin(freq * t + 3.0);
        pixel.x = (unsigned char)(255 * (r_val * r_val));
        pixel.y = (unsigned char)(128 + 127 * sin(freq * t + 1.5));
        pixel.z = (unsigned char)(200 + 55 * sin(freq * t + 0.5));
        pixel.w = 255;
    } else {
        pixel = (uchar4)(0, 0, 0, 255);
    }

    image[idx] = pixel;
}