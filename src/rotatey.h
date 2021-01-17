#ifndef ROTATEY_H
#define ROTATEY_H

#include <memory>
#include <hittable.h>

class RotateY : public Hittable
{
public:
    RotateY(std::shared_ptr<Hittable> p, double angle);

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

private:
    std::shared_ptr<Hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    Aabb bbox;
};

#endif
