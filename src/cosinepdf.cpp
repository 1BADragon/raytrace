#include <cosinepdf.h>
#include <common.h>

CosinePdf::CosinePdf(const Vec3 &w)
{
    uvw.build_from_w(w);
}

double CosinePdf::value(const Vec3 &direction) const
{
    auto cosine = direction.unit_vector().dot(uvw.w());
    return (cosine <= 0) ? 0 : cosine / pi;
}

Vec3 CosinePdf::generate() const
{
    return uvw.local(random_cosine_direction());
}
