#include <hitrecord.h>
#include <hittablelist.h>

bool HittableList::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    bool hit_anything = false;
    double closest_so_far = max;

    for (const auto &object : objects) {
        auto tmp_rec = HitRecord();
        if (object->hit(r, min, closest_so_far, tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_anything;
}
