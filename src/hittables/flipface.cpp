#include <ray.h>
#include <hitrecord.h>
#include <hittables/flipface.h>

bool FlipFace::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    if (!ptr->hit(r, min, max, rec)) {
        return false;
    }

    rec.front_face = !rec.front_face;
    return true;
}

bool FlipFace::bounding_box(double time0, double time1, Aabb &output_box) const
{
    return ptr->bounding_box(time0, time1, output_box);
}

int FlipFace::n_children() const
{
    return ptr->n_children() + 1;
}
