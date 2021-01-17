#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <memory>
#include <vec3.h>
#include <hittable.h>

class Translate : public Hittable
{
public:
    Translate(std::shared_ptr<Hittable> p, const Vec3& displacement) :
        ptr(p), offset(displacement) {}

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

private:
    std::shared_ptr<Hittable> ptr;
    Vec3 offset;
};

#endif
