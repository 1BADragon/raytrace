#include <materials/isotropic.h>
#include <ray.h>
#include <hitrecord.h>

bool Isotropic::scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    scattered = Ray(rec.p, Vec3::random_in_unit_sphere(), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}
