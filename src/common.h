#ifndef COMMON_H
#define COMMON_H

#include <limits>
#include <cmath>
#include <vec3.h>

#define UNUSED __attribute__((unused))

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = M_PI;

int random_int(int min, int max);
double degrees_to_radians(double degrees);
double random_double(void);
double random_double(double min, double max);
double clamp(double val, double min, double max);
Vec3 random_cosine_direction();
Vec3 random_to_sphere(double radius, double distance_squared);

#endif // COMMON_H
