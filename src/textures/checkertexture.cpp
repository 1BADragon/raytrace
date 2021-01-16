#include <textures/checkertexture.h>

Color CheckerTexture::value(double u, double v, const Point3 &p) const
{
    auto sines = std::sin(10 * p.x()) * std::sin(10 * p.y()) *
            std::sin(10 * p.z());
    if (sines < 0) {
        return odd->value(u, v, p);
    } else {
        return even->value(u, v, p);
    }
}
