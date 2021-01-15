#include "ray.h"

Ray::Ray() :
    orig(), dir()
{

}

Ray::Ray(const Point3 &orig, const Vec3 &dir) :
    orig(orig), dir(dir)
{

}

Point3 Ray::at(double t) const
{
    return origin() + t * direction();
}
