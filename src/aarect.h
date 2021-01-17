#ifndef XYRECT_H
#define XYRECT_H

#include <memory>
#include <materials/material.h>
#include <common.h>
#include <hittable.h>

class XYRect : public Hittable {
public:
    XYRect() {}
    XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
           std::shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

private:
    double x0;
    double x1;
    double y0;
    double y1;
    double k;
    std::shared_ptr<Material> mp;
};

class XZRect : public Hittable {
    public:
        XZRect() {}

        XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
            std::shared_ptr<Material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

        virtual bool bounding_box(double time0, double time1, Aabb& output_box) const override;

    public:
        double x0;
        double x1;
        double z0;
        double z1;
        double k;
        std::shared_ptr<Material> mp;
};

class YZRect : public Hittable {
    public:
        YZRect() {}

        YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
            std::shared_ptr<Material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

        virtual bool bounding_box(double time0, double time1, Aabb& output_box) const override;

    public:
        double y0;
        double y1;
        double z0;
        double z1;
        double k;
        std::shared_ptr<Material> mp;
};

#endif
