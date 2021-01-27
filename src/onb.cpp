#include <cmath>
#include <onb.h>

Vec3 Onb::local(double a, double b, double c) const
{
    return a*u() + b*v() + c*w();
}

Vec3 Onb::local(const Vec3 &a) const
{
    return a.x()*u() + a.y()*v() + a.z()*w();
}

void Onb::build_from_w(const Vec3 &n)
{
    axis[2] = n.unit_vector();
    Vec3 a = (std::fabs(w().x()) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
    axis[1] = (w().cross(a)).unit_vector();
    axis[0] = w().cross(v());
}
