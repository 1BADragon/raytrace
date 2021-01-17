#ifndef MATERIAL_H
#define MATERIAL_H

#include <color.h>

class Ray;
class HitRecord;

class Material
{
public:
    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         Color &attenuation, Ray &scattered) const = 0;

    virtual Color emitted(double u, double v, const Point3 &p) const {
        (void)u;
        (void)v;
        (void)p;
        return Color(0, 0, 0);
    }
};

#endif // MATERIAL_H
