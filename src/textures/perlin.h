#ifndef PERLIN_H
#define PERLIN_H

#include <array>
#include <common.h>
#include <vec3.h>

class Perlin {
public:
    Perlin();

    double noise(const Point3 &p) const;

    double turb(const Point3 &p, int depth=7) const;

private:
    static constexpr int point_count = 256;

    std::array<Vec3, point_count> ranvec;
    std::array<int, point_count> perm_x;
    std::array<int, point_count> perm_y;
    std::array<int, point_count> perm_z;

    static void perlin_generate_perm(std::array<int, point_count> &perm);
    static void permute(std::array<int, point_count> &perm);
    static double perlin_interp(Vec3 c[2][2][2], double u, double v, double w);
};


#endif
