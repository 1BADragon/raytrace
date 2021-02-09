#include <cmath>
#include <onb.h>
#include <hitrecord.h>
#include <hittables/sphere.h>

Sphere::Sphere(const Point3 &center, double r, std::shared_ptr<Material> m) :
    center(center), radius(r), mat_ptr(m)
{

}

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
    auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat_ptr;

    return true;
}

bool Sphere::bounding_box(double _time0, double _time1, Aabb &output_box) const
{
    Aabb box(
                center - Vec3(radius, radius, radius),
                center + Vec3(radius, radius, radius));

    output_box = box;

    return true;
}

double Sphere::pdf_value(const Point3 &o, const Vec3 &v) const
{
    HitRecord rec;
    if (!this->hit(Ray(o, v), 0.001, infinity, rec)) {
        return 0;
    }

    auto cos_theta_max = std::sqrt(1 - radius*radius/(center-o).length_squared());
    auto solid_angle = 2*pi*(1-cos_theta_max);

    return 1. / solid_angle;
}

Vec3 Sphere::random(const Point3 &o) const
{
    Vec3 direction = center - o;
    auto distance_squared = direction.length_squared();
    Onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}

void Sphere::get_sphere_uv(const Point3 &p, double &u, double &v) const
{
    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2*pi);
    v = theta / pi;
}
