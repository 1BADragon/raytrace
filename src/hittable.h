#ifndef HITTABLE_H
#define HITTABLE_H

#include <ray.h>

struct HitRecord;

class Hittable {
public:
    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const = 0;
};

#endif // HITTABLE_H
