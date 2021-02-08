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
                       std::shared_ptr<Hittable> world, std::shared_ptr<Hittable> lights, int depth)
{
    if (depth <= 0) {
        return Color();
    }

    auto rec = HitRecord();

    if (!world->hit(r, 0.001, infinity, rec)) {
        return background;
    }

    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);
    double pdf_val;
    Color albedo;

    if (!rec.mat->scatter(r, rec, albedo, scattered, pdf_val)) {
        return emitted;
    }

    auto p0 = std::make_shared<HittablePdf>(lights, rec.p);
    auto p1 = std::make_shared<CosinePdf>(rec.normal);
    MixturePdf mixed_pdf(p0, p1);

    scattered = Ray(rec.p, mixed_pdf.generate(), r.time());
    pdf_val = mixed_pdf.value(scattered.direction());

    auto scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);
    auto color = ray_color(scattered, background, world, lights, depth-1);

    auto val = emitted + albedo * scattering_pdf * color / pdf_val;

    return val;
}


void write_color(std::ostream &s, const Color pixel_color, size_t samples_per_pixel)
{
    double scale = 1.0 / static_cast<double>(samples_per_pixel);
    auto r = std::sqrt(pixel_color.x() * scale);
    auto g = std::sqrt(pixel_color.y() * scale);
    auto b = std::sqrt(pixel_color.z() * scale);

    int ir = static_cast<int>(256. * clamp(r, 0.0, 0.999));
    int ig = static_cast<int>(256. * clamp(g, 0.0, 0.999));
    int ib = static_cast<int>(256. * clamp(b, 0.0, 0.999));

    if (ir < 0 || ir > 255) {
        throw std::runtime_error("Invalid value for ir: " + std::to_string(ir) + ", " +
                                 std::to_string(r));
    }

    if (ig < 0 || ig > 255) {
        throw std::runtime_error("Invalid value for ir: " + std::to_string(ig) + ", " +
                                 std::to_string(g));
    }

    if (ib < 0 || ib > 255) {
        throw std::runtime_error("Invalid value for ir: " + std::to_string(ib) + ", " +
                                 std::to_string(b));
    }

    s << ir << ' ' << ig << ' ' << ib << '\n';
}
