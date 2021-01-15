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
};

#endif // MATERIAL_H
