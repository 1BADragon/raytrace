#ifndef HITTABLE_H
#define HITTABLE_H

#include <ray.h>
#include <aabb.h>

struct HitRecord;

class Hittable {
public:
    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const = 0;
    virtual bool bounding_box(double time0, double time1, Aabb& output_box) const = 0;
};

#endif // HITTABLE_H
