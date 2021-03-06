#include <random>
#include <thread>
#include <cstdlib>
#include <common.h>

double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

double random_double() {
    static thread_local std::default_random_engine rng(std::hash<std::thread::id>()(std::this_thread::get_id()));


    // Returns a random real in [0,1).
    return rng() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max+1));
}

Vec3 random_cosine_direction()
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = std::sqrt(1 - r2);

    auto phi = 2*pi*r1;
    auto x = std::cos(phi)*std::sqrt(r2);
    auto y = std::sin(phi)*std::sqrt(r2);

    return Vec3(x, y, z);
}

Vec3 random_to_sphere(double radius, double distance_squared)
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2*(std::sqrt(1-radius*radius/distance_squared) - 1);

    auto phi = 2*pi*r1;
    auto x = std::cos(phi)*std::sqrt(1-z*z);
    auto y = std::sin(phi)*std::sqrt(1-z*z);

    return Vec3(x, y, z);
}
