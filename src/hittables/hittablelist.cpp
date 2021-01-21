#include <hitrecord.h>
#include <hittables/hittablelist.h>

bool HittableList::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    bool hit_anything = false;
    double closest_so_far = max;

    for (const auto &object : _objects) {
        auto tmp_rec = HitRecord();
        if (object->hit(r, min, closest_so_far, tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::bounding_box(double time0, double time1, Aabb &output_box) const
{
    if (_objects.empty()) {
        return false;
    }

    Aabb temp_box;
    bool first_box = true;

    for (const auto &object : _objects) {
        if (!object->bounding_box(time0, time1, temp_box)) {
            return false;
        }
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

const std::vector<std::shared_ptr<Hittable> > &HittableList::objects() const
{
    return _objects;
}
