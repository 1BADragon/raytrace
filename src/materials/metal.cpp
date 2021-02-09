#include <ray.h>
#include <hitrecord.h>
#include <materials/metal.h>

Metal::Metal(const Color &a, double f) :
    albedo(a), fuzz((f < 1.)? f : 1.)
{

}

bool Metal::scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord &srec) const
{
    auto reflected = Vec3::reflect(r_in.direction().unit_vector(),
                                   rec.normal);
    srec.specular_ray = Ray(rec.p, reflected+fuzz*Vec3::random_in_unit_sphere());
    srec.attenuation = albedo;
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    return true;
}
