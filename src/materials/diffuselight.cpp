#include <materials/diffuselight.h>
#include <hitrecord.h>

Color DiffuseLight::emitted(const Ray &r_in, const HitRecord &rec, double u, double v, const Point3 &p) const
{
    (void) r_in;
    if (rec.front_face) {
        return emit->value(u, v, p);
    } else {
        return Color(0, 0, 0);
    }
}
