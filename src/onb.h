#ifndef ONB_H
#define ONB_H

#include <vec3.h>

class Onb
{
public:
    Onb() {}

    inline Vec3 operator[](size_t i) const { return axis[i]; }

    Vec3 u() const { return axis[0]; }
    Vec3 v() const { return axis[1]; }
    Vec3 w() const { return axis[2]; }

    Vec3 local(double a, double b, double c) const;
    Vec3 local(const Vec3 &a) const;
    void build_from_w(const Vec3 &n);

private:
    Vec3 axis[3];
};

#endif // ONB_H
