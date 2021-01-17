#ifndef BOX_H
#define BOX_H

#include <common.h>
#include <aarect.h>
#include <hittablelist.h>

class Box : public Hittable
{
public:
    Box() {}
    Box(const Point3 &p0, const Point3 &p1, std::shared_ptr<Material> ptr);

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

private:
    Point3 box_min;
    Point3 box_max;
    HittableList sides;
};

#endif
