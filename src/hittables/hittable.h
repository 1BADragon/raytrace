#ifndef HITTABLE_H
#define HITTABLE_H

#include <ray.h>
#include <aabb.h>
#include <typeinfo>
#include <iostream>

struct HitRecord;

class Hittable {
public:
    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const = 0;
    virtual bool bounding_box(double time0, double time1, Aabb& output_box) const = 0;
    virtual double pdf_value(const Point3& o, const Vec3 &v) const {
        (void) o;
        (void) v;

        return 0.0;
    }

    virtual Vec3 random(const Vec3 &o) const {
        (void) o;

        return Vec3(1, 0, 0);
    }
};

#endif // HITTABLE_H
