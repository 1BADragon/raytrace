#include <sstream>
#include <cmath>
#include <color.h>
#include <common.h>

void write_color(std::ostream &s, const Color pixel_color, size_t samples_per_pixel)
{
    double scale = 1.0 / static_cast<double>(samples_per_pixel);
    auto r = std::sqrt(pixel_color.x() * scale);
    auto g = std::sqrt(pixel_color.y() * scale);
    auto b = std::sqrt(pixel_color.z() * scale);

    s << static_cast<int>(256. * clamp(r, 0.0, 0.999)) << ' ' <<
         static_cast<int>(256. * clamp(g, 0.0, 0.999)) << ' ' <<
         static_cast<int>(256. * clamp(b, 0.0, 0.999)) << '\n';
}
