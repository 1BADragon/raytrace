
#include <textures/solidcolor.h>

Color SolidColor::value(double u, double v, const Point3 &p) const
{
    (void)u;
    (void)v;
    (void)p;
    return color_value;
}
