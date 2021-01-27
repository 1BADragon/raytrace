#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include <materials/material.h>

class Dielectric : public Material
{
public:
    Dielectric(double index_of_refraction): ir(index_of_refraction) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const;

private:
    double ir;

    double reflectance(double cosine, double ref_idx) const;
};

#endif
