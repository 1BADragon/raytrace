#include <hittables/aarect.h>
#include <hitrecord.h>

bool XYRect::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    auto t = (k-r.origin().z()) / r.direction().z();
    if (t < min || t > max) {
        return false;
    }
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = Vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat = mp;
    rec.p = r.at(t);
    return true;
}

bool XYRect::bounding_box(double time0, double time1, Aabb &output_box) const
{
    (void)time0;
    (void)time1;
    output_box = Aabb(Point3(x0, y0, k-0.0001), Point3(x1, y1, k+0.0001));
    return true;
}

bool XZRect::bounding_box(double time0, double time1, Aabb& output_box) const {
    (void)time0;
    (void)time1;
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount.
    output_box = Aabb(Point3(x0,k-0.0001,z0), Point3(x1, k+0.0001, z1));
    return true;
}

double XZRect::pdf_value(const Point3 &origin, const Vec3 &v) const
{
    HitRecord rec;
    if (!this->hit(Ray(origin, v), 0.001, infinity, rec)) {
        return 0;
    }

    auto area = (x1-x0)*(z1-z0);
    auto distance_squared = rec.t * rec.t * v.length_squared();
    auto cosine = std::fabs(v.dot(rec.normal) / v.length());

    return distance_squared / (cosine * area);
}

Vec3 XZRect::random(const Vec3 &o) const
{
    auto random_point = Point3(random_double(x0, x1), k, random_double(z0, z1));
    return random_point - o;
}

bool XZRect::hit(const Ray &r, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (k-r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = Vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat = mp;
    rec.p = r.at(t);
    return true;
}

bool YZRect::bounding_box(double time0, double time1, Aabb& output_box) const
{
    (void)time0;
    (void)time1;
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    output_box = Aabb(Point3(k-0.0001, y0, z0), Point3(k+0.0001, y1, z1));
    return true;
}

bool YZRect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    auto t = (k-r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = r.origin().y() + t*r.direction().y();
    auto z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = Vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat = mp;
    rec.p = r.at(t);
    return true;
}
