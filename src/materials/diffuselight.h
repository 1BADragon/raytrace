#ifndef DIFFUSELIGHT_H
#define DIFFUSELIGHT_H

#include <memory>
#include <materials/material.h>
#include <textures/texture.h>
#include <textures/solidcolor.h>

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> a) :
        emit(a) {}

    DiffuseLight(const Color &c) :
        emit(std::make_shared<SolidColor>(c)) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, Color &albedo, Ray &scattered, double &pdf) const override;

    virtual Color emitted(const Ray &r_in, const HitRecord &rec, double u, double v, const Point3& p) const override;

private:
    std::shared_ptr<Texture> emit;
};

#endif
