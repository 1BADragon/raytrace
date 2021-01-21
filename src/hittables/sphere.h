#ifndef SPHERE_H
#define SPHERE_H

#include <memory>
#include <hittables/hittable.h>

class Material;

class Sphere : public Hittable
{
public:
    Sphere() {}
    Sphere(const Point3 &center, double r, std::shared_ptr<Material> m);
    Sphere(const Point3 &cen0, const Point3 &cen1, double _time0, double _time1, double r, std::shared_ptr<Material> m);

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;
    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

    Point3 center(double time) const;
    void get_sphere_uv(const Point3& p, double &u, double &v) const;

private:
    Point3 center0;
    Point3 center1;
    double time0;
    double time1;
    double radius;
    bool moving;
    std::shared_ptr<Material> mat_ptr;
};

#endif // SPHERE_H
