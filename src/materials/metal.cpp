#include <ray.h>
#include <hitrecord.h>
#include <materials/metal.h>

Metal::Metal(const Color &a, double f) :
    albedo(a), fuzz((f < 1.)? f : 1.)
{

}

bool Metal::scatter(const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const
{
    auto reflected = Vec3::reflect(r_in.direction().unit_vector(),
                                   rec.normal);
    scattered = Ray(rec.p, reflected + this->fuzz * Vec3::random_unit_sphere());
    attenuation = this->albedo;
    return scattered.direction().dot(rec.normal) > 0;
}
