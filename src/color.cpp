#include <sstream>
#include <algorithm>
#include <cmath>
#include <color.h>
#include <common.h>
#include <hitrecord.h>
#include <materials/material.h>
#include <hittablepdf.h>
#include <cosinepdf.h>
#include <mixturepdf.h>
#include <mutex>


Color ray_color(const Ray &r, const Color &background,
                       std::shared_ptr<Hittable> world, std::shared_ptr<HittableList> lights, int depth)
{
    if (depth <= 0) {
        return Color();
    }

    auto rec = HitRecord();

    if (!world->hit(r, 0.001, infinity, rec)) {
        return background;
    }

    ScatterRecord srec;
    Color emitted = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!rec.mat->scatter(r, rec, srec)) {
        return emitted;
    }

    if (srec.is_specular) {
        return srec.attenuation * ray_color(srec.specular_ray, background, world, lights, depth-1);
    }

    std::shared_ptr<Pdf> pdf = srec.pdf_ptr;

    if (lights->n_objects()) {
        auto light_ptr = std::make_shared<HittablePdf>(lights, rec.p);
        auto p = std::make_shared<MixturePdf>(light_ptr, srec.pdf_ptr);
        pdf = p;
    }

    Ray scattered = Ray(rec.p, pdf->generate(), r.time());

    auto pdf_val = pdf->value(scattered.direction());
    auto scattering = rec.mat->scattering_pdf(r, rec, scattered);

    return emitted
            + srec.attenuation * scattering
            * ray_color(scattered, background, world, lights, depth-1) / pdf_val;
}


void write_color(std::ostream &s, const Color pixel_color)
{
    s << pixel_color.x() << ' ' << pixel_color.y()
      << ' ' << pixel_color.z() << '\n';
}
