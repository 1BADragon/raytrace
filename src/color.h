#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <vec3.h>

using Color = Vec3;

void write_color(std::ostream &s, const Color pixel_color, size_t samples_per_pixel);

#endif // COLOR_H
