#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include <memory>
#include <common.h>
#include <hittables/hittable.h>
#include <materials/material.h>
#include <textures/texture.h>
#include <materials/isotropic.h>

class ConstantMedium : public Hittable
{
public:
    ConstantMedium(std::shared_ptr<Hittable> b, double d, std::shared_ptr<Texture> a) :
        boundary(b), neg_inv_density(-1./d), phase_function(std::make_shared<Isotropic>(a)) {}

    ConstantMedium(std::shared_ptr<Hittable> b, double d, Color c) :
        boundary(b), neg_inv_density(-1./d), phase_function(std::make_shared<Isotropic>(c)) {}

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

    virtual int n_children() const override;

private:
    std::shared_ptr<Hittable> boundary;
    double neg_inv_density;
    std::shared_ptr<Material> phase_function;    
};

#endif
