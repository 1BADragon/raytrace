#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include <hittable.h>

class Material;

class Sphere : public Hittable
{
public:
    Sphere(const Point3 &center, double r, std::shared_ptr<Material> mat) :
        center(center), radius(r), material(mat) {}

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> material;
};

#endif // SPHERE_H
