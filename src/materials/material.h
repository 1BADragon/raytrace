#ifndef MATERIAL_H
#define MATERIAL_H

#include <color.h>

class Ray;
class HitRecord;

class Material
{
public:
    virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                         Color &albedo, Ray &scattered, double &pdf) const {
        (void) r_in;
        (void) rec;
        (void) albedo;
        (void) scattered;
        (void) pdf;
        return false;
    }

    virtual double scattering_pdf(
            const Ray& r_in, const HitRecord& rec, const Ray& scattered
            ) const {
        (void) r_in;
        (void) rec;
        (void) scattered;
        return 0;
    }

    virtual Color emitted(const Ray &r_in, const HitRecord &rec, double u, double v, const Point3 &p) const {
        (void)r_in;
        (void)rec;
        (void)u;
        (void)v;
        (void)p;
        return Color(0, 0, 0);
    }
};

#endif // MATERIAL_H
