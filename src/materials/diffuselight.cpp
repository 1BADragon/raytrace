#include <materials/diffuselight.h>

bool DiffuseLight::scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    (void)r_in;
    (void)rec;
    (void)attenuation;
    (void)scattered;
    return false;
}

Color DiffuseLight::emitted(double u, double v, const Point3 &p) const
{
    return emit->value(u, v, p);
}
