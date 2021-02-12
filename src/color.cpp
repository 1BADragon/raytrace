#include <sstream>
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


void write_color(std::ostream &s, const Color pixel_color, size_t samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    s << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
