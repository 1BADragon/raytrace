#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include <memory>
#include <materials/material.h>
#include <color.h>
#include <textures.h>

class HitRecord;
class Ray;

class Lambertian : public Material
{
public:
    Lambertian(const Color &a): albedo(std::make_shared<SolidColor>(a)) {}
    Lambertian(std::shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override;

private:
    std::shared_ptr<Texture> albedo;
};

#endif
