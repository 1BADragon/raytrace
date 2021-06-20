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

double HittableList::pdf_value(const Vec3 &o, const Vec3 &v) const
{
    auto weight = 1.0 / n_objects();
    auto sum = 0.0;

    for (const auto& object : _objects) {
        sum += weight * object->pdf_value(o, v);
    }

    return sum;
}

int HittableList::n_children() const
{
    int tmp = n_objects();

    for (const auto &c : _objects) {
        tmp += c->n_children();
    }

    return tmp;
}

Vec3 HittableList::random(const Vec3 &o) const
{
    auto int_size = static_cast<int>(n_objects());
    if (_objects.size()) {
        return _objects[random_int(0, int_size-1)]->random(o);
    } else {
        return Hittable::random(o);
    }
}

const std::vector<std::shared_ptr<Hittable> > &HittableList::objects() const
{
    return _objects;
}
