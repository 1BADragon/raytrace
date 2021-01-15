#include <cmath>
#include <hitrecord.h>
#include <sphere.h>

bool Sphere::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    auto oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = oc.dot(r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminat = half_b * half_b - a * c;

    if (discriminat < 0.) {
        return false;
    }

    auto sqrtd = std::sqrt(discriminat);

    // Find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a;
    if (root < min || max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < min || max < root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = material;

    return true;
}
