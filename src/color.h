#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <memory>
#include <vec3.h>
#include <ray.h>
#include <hittables/hittable.h>
#include <hittables/hittablelist.h>

using Color = Vec3;

void write_color(std::ostream &s, const Color pixel_color, size_t samples_per_pixel);

Color ray_color(const Ray &r, const Color &background,
                std::shared_ptr<Hittable> world, std::shared_ptr<HittableList> lights, int depth);

#endif // COLOR_H
