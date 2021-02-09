#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include <vec3.h>
#include <hittables/hittable.h>

class Material;

class Sphere : public Hittable
{
public:
    Sphere() {}
    Sphere(const Point3 &center, double r, std::shared_ptr<Material> m);

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;
    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;
    virtual double pdf_value(const Point3 &o, const Vec3 &v) const override;

    virtual Vec3 random(const Point3 &o) const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat_ptr;

    void get_sphere_uv(const Point3& p, double &u, double &v) const;
};

#endif // SPHERE_H
