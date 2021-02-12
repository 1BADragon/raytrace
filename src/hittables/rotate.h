#ifndef ROTATEY_H
#define ROTATEY_H

#include <memory>
#include <hittables/hittable.h>
#include <mat3.h>

class Rotate : public Hittable
{
public:
    Rotate(std::shared_ptr<Hittable> p, double x, double y, double z);

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

private:
    std::shared_ptr<Hittable> ptr;
    Mat3 _transform;
    bool hasbox;
    Aabb bbox;
};

#endif
