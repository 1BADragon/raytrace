#ifndef AABB_H
#define AABB_H

#include <common.h>
#include <vec3.h>
#include <ray.h>

class Aabb {
public:
    Aabb() {}
    Aabb(const Point3 &a, const Point3 &b) :
        minimum(a), maximum(b) {}

    Point3 min() const { return minimum; }
    Point3 max() const { return maximum; }

    bool hit(const Ray& r, double t_min, double t_max) const;

    Point3 minimum;
    Point3 maximum;
};

Aabb surrounding_box(const Aabb &box0, const Aabb &box1);

#endif
