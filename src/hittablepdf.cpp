#include <hittablepdf.h>

HittablePdf::HittablePdf(std::shared_ptr<Hittable> p, const Point3 &origin) :
    o(origin), ptr(p)
{

}

double HittablePdf::value(const Vec3 &direction) const
{
    return ptr->pdf_value(o, direction);
}

Vec3 HittablePdf::generate() const
{
    return ptr->random(o);
}

