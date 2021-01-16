#include <materials/lambertian.h>
#include <hitrecord.h>
#include <ray.h>
#include <vec3.h>

bool Lambertian::scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    (void)r_in;
    auto scatter_direction = rec.normal + Vec3::random_unit_vec();

    if (scatter_direction.near_zero()) {
        scatter_direction = rec.normal;
    }

    scattered = Ray(rec.p, scatter_direction, r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}
