#include <common.h>
#include <materials/lambertian.h>
#include <cosinepdf.h>
#include <hitrecord.h>
#include <ray.h>
#include <vec3.h>
#include <onb.h>

bool Lambertian::scatter(const Ray &r_in UNUSED, const HitRecord &rec, ScatterRecord &srec) const
{
    srec.is_specular = false;
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = std::make_shared<CosinePdf>(rec.normal);
    return true;
}

double Lambertian::scattering_pdf(const Ray &r_in, const HitRecord &rec, const Ray &scattered) const
{
    (void)r_in;
    auto cosine = rec.normal.dot(scattered.direction().unit_vector());
    return cosine < 0 ? 0 : cosine / pi;
}
