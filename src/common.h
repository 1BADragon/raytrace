#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <cmath>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = M_PI;

double degrees_to_radians(double degrees);
double random_double(void);
double random_double(double min, double max);
double clamp(double val, double min, double max);

#endif // COMMON_H
