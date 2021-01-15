#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include <materials/material.h>
#include <color.h>

class HitRecord;
class Ray;

class Lambertian : public Material
{
public:
    Lambertian(const Color &a): albedo(a) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

private:
    Color albedo;
};

#endif
