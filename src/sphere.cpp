#include <cmath>
#include <hitrecord.h>
#include <sphere.h>

Sphere::Sphere(const Point3 &center, double r, std::shared_ptr<Material> m) :
    center0(center), center1(center), time0(0.), time1(0.),
    radius(r), moving(false), mat_ptr(m)
{

}

Sphere::Sphere(const Point3 &cen0, const Point3 &cen1, double _time0, double _time1, double r, std::shared_ptr<Material> m) :
    center0(cen0), center1(cen1), time0(_time0), time1(_time1),
    radius(r), moving(true), mat_ptr(m)
{

}

bool Sphere::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    auto oc = r.origin() - center(r.time());
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
    auto outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat_ptr;

    return true;
}

bool Sphere::bounding_box(double _time0, double _time1, Aabb &output_box) const
{
    Aabb box0(
                center(_time0) - Vec3(radius, radius, radius),
                center(_time0) + Vec3(radius, radius, radius));
    Aabb box1(
                center(_time1) - Vec3(radius, radius, radius),
                center(_time1) - Vec3(radius, radius, radius));

    output_box = surrounding_box(box0, box1);

    return true;
}

Point3 Sphere::center(double time) const
{
    if (moving) {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    } else {
        return center0;
    }
}

void Sphere::get_sphere_uv(const Point3 &p, double &u, double &v) const
{
    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2*pi);
    v = theta / pi;
}
