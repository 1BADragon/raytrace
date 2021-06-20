#include <randompdf.h>
#include <common.h>


double RandomPdf::value(const Vec3 &direction) const
{
    return random_double();
}

Vec3 RandomPdf::generate() const
{
    return Vec3::random_in_unit_sphere();
}
