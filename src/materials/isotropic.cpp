#include <materials/isotropic.h>
#include <ray.h>
#include <hitrecord.h>
#include <randompdf.h>

bool Isotropic::scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord &srec) const
{ 
    srec.is_specular = false;
    srec.specular_ray = Ray(rec.p, Vec3::random_in_unit_sphere(), r_in.time());
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = std::make_shared<RandomPdf>();
    return true;
}
