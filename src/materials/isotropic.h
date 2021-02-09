#ifndef ISOTROPIC_H
#define ISOTROPIC_H

#include <memory>
#include <scatterrecord.h>
#include <textures/texture.h>
#include <textures/solidcolor.h>
#include <materials/material.h>

class Isotropic : public Material
{
public:
    Isotropic(const Color &c) :
        albedo(std::make_shared<SolidColor>(c)) {}
    Isotropic(std::shared_ptr<Texture> a) :
        albedo(a) {}

    virtual bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord &srec) const override;

private:
    std::shared_ptr<Texture> albedo;
};


#endif
