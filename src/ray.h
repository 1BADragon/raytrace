#ifndef RAY_H
#define RAY_H

#include <vec3.h>

class Ray
{
public:
    Ray();
    Ray(const Point3 &orig, const Vec3 &dir, double time=0.0);

    Point3 origin() const { return orig; }
    Vec3 direction() const { return dir; }
    double time() const { return tm; }

    Point3 at(double t) const;

private:
    Point3 orig;
    Vec3 dir;
    double tm;
};

#endif // RAY_H
