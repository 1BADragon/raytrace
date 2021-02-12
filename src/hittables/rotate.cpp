#include <hittables/rotate.h>
#include <common.h>
#include <hitrecord.h>

Rotate::Rotate(std::shared_ptr<Hittable> p, double x, double y, double z) :
    ptr(p)
{
    auto a = degrees_to_radians(x);
    auto b = degrees_to_radians(y);
    auto g = degrees_to_radians(z);

    _transform[0] = {cos(a) * cos(b), cos(a)*sin(b)*sin(g) - sin(a)*cos(g), cos(a)*sin(b)*cos(g) + sin(a)*sin(g)};
    _transform[1] = {sin(a)*cos(b), sin(a)*sin(b)*sin(g) + cos(a)*cos(g), sin(a)*sin(b)*cos(g) - cos(a)*sin(g)};
    _transform[2] = {-sin(b), cos(b)*sin(g), cos(b)*cos(g)};

    hasbox = ptr->bounding_box(0, 1, bbox);

    Point3 min( infinity, infinity, infinity);
    Point3 max( -infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                Point3 new_point = _transform.cross({x, y, z});

                for (int c = 0; c < 3; c++) {
                    min[c] = std::fmin(min[c], new_point[c]);
                    max[c] = std::fmin(max[c], new_point[c]);
                }
            }
        }
    }

    bbox = Aabb(min, max);
}

bool Rotate::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    auto origin = r.origin();
    auto direction = r.direction();

    origin = _transform.cross(origin);
    direction = _transform.cross(direction);

    Ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, min, max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p = _transform.cross(rec.p);
    normal = _transform.cross(rec.normal);

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

bool Rotate::bounding_box(double time0, double time1, Aabb &output_box) const
{
    (void)time0;
    (void)time1;
    output_box = bbox;
    return hasbox;
}
