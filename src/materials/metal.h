#ifndef METAL_H
#define METAL_H

#include <color.h>
#include <materials/material.h>

class Metal : public Material
{
public:
    Metal(const Color &a, double f);

    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         Color &attenuation, Ray &scattered) const;
private:
    Color albedo;
    double fuzz;
};

#endif // METAL_H
