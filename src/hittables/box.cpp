#include <hittables/box.h>

Box::Box(const Point3 &p0, const Point3 &p1, std::shared_ptr<Material> ptr)
{
    sides.add(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool Box::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    return sides.hit(r, min, max, rec);
}

bool Box::bounding_box(double time0, double time1, Aabb &output_box) const
{
    (void) time0;
    (void) time1;
    output_box = Aabb(box_min, box_max);
    return true;
}

int Box::n_children() const
{
    return sides.n_children();
}
