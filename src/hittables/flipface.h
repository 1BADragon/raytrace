#ifndef FLIPFACE_H
#define FLIPFACE_H

#include <memory>
#include <hittables/hittable.h>

class FlipFace : public Hittable
{
public:
    FlipFace(std::shared_ptr<Hittable> p) :
        ptr(p) {}

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

    virtual int n_children() const override;

private:
    std::shared_ptr<Hittable> ptr;
};

#endif // FLIPFACE_H
