#include <cstdlib>
#include <common.h>

double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

double clamp(double val, double min, double max)
{
    if (val < min) {
        return min;
    } else if (val > max) {
        return max;
    } else {
        return val;
    }
}

int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max+1));
}
