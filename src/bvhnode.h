#ifndef BVHNODE_H
#define BVHNODE_H

#include <memory>
#include <vector>
#include <ray.h>
#include <common.h>
#include <hittable.h>
#include <hittablelist.h>

class BvhNode : public Hittable
{
public:
    BvhNode();

    BvhNode(const std::shared_ptr<HittableList> list, double time0, double time1)
        : BvhNode(list->objects(), 0, list->objects().size(), time0, time1)
    {}

    BvhNode(const std::vector<std::shared_ptr<Hittable>>& src_objects,
            size_t start, size_t end, double time0, double time1);

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb& output_box) const override;

public:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    Aabb box;
};

bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis);

bool box_x_compare (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);

bool box_y_compare (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);

bool box_z_compare (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);

#endif
