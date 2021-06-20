#include <hittables/translate.h>
#include <hitrecord.h>

bool Translate::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    Ray moved_r(r.origin() - offset, r.direction(), r.time());

    if (!ptr->hit(moved_r, min, max, rec)) {
        return false;
    }

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool Translate::bounding_box(double time0, double time1, Aabb &output_box) const
{
    if (!ptr->bounding_box(time0, time1, output_box)) {
        return false;
    }

    output_box = Aabb(output_box.min() + offset,
                      output_box.max() + offset);

    return true;
}

int Translate::n_children() const
{
    return ptr->n_children() + 1;
}
