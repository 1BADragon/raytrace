#include <materials/lambertian.h>
#include <hitrecord.h>
#include <ray.h>
#include <vec3.h>
#include <onb.h>

bool Lambertian::scatter(const Ray &r_in, const HitRecord &rec, Color &albedo,
                         Ray &scattered, double &pdf) const
{
    Onb uvw;
    uvw.build_from_w(rec.normal);
    auto direction = uvw.local(random_cosine_direction());
    scattered = Ray(rec.p, direction.unit_vector(), r_in.time());
    albedo = this->albedo->value(rec.u, rec.v, rec.p);
    pdf = uvw.w().dot(scattered.direction()) / pi;
    return true;
}

double Lambertian::scattering_pdf(const Ray &r_in, const HitRecord &rec, const Ray &scattered) const
{
    (void)r_in;
    auto cosine = rec.normal.dot(scattered.direction().unit_vector());
    return cosine < 0 ? 0 : cosine / pi;
}
