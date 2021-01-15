#include <cmath>
#include <materials/dielectric.h>
#include <ray.h>
#include <hitrecord.h>
#include <color.h>
#include <common.h>

bool Dielectric::scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    attenuation = Color(1.0, 1.0, 1.0);

    auto refraction_radio = (rec.front_face)? 1.0 / this->ir : this->ir;
    auto unit_direction = r_in.direction().unit_vector();
    auto cos_theta = std::min((-unit_direction).dot(rec.normal), 1.0);
    auto sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    auto cannot_refract = refraction_radio * sin_theta > 1.0;
    Vec3 direction;
    if (cannot_refract ||
            reflectance(cos_theta, refraction_radio) > random_double()) {
        direction = Vec3::reflect(unit_direction, rec.normal);
    } else {
        direction = Vec3::refract(unit_direction, rec.normal, refraction_radio);
    }

    scattered = Ray(rec.p, direction);
    return true;
}

double Dielectric::reflectance(double cosine, double ref_idx) const
{
    auto r0 = (1. - ref_idx) / (1. + ref_idx);
    r0 = r0 * r0;
    return r0 + (1. - r0) * std::pow((1. - cosine), 5.);
}

