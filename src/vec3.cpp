#include <random>
#include <algorithm>
#include <vec3.h>
#include <common.h>

Vec3::Vec3() :
    _x(0), _y(0), _z(0)
{

}

Vec3::Vec3(double x, double y, double z) :
    _x(x), _y(y), _z(z)
{

}

Vec3 Vec3::random()
{
    return Vec3(random_double(), random_double(), random_double());
}

Vec3 Vec3::random_ranged(double min, double max)
{
    return Vec3(random_double(min, max),
                random_double(min, max),
                random_double(min, max));
}

Vec3 Vec3::random_in_unit_sphere()
{
    for (;;) {
        auto p = Vec3::random_ranged(-1., 1.);
        if (p.length_squared() >= 1.) {
            continue;
        }
        return p;
    }
}

Vec3 Vec3::random_in_hemi(const Vec3 &normal)
{
    auto unit_sphere = Vec3::random_in_unit_sphere();
    if (unit_sphere.dot(normal) > 0.0) {
        return unit_sphere;
    } else {
        return -unit_sphere;
    }
}

Vec3 Vec3::random_unit_vec() {
    return random_in_unit_sphere().unit_vector();
}

Vec3 Vec3::reflect(const Vec3 &v, const Vec3 &n)
{
    return v - 2. * v.dot(n) * n;
}

Vec3 Vec3::refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat)
{
    auto cos_theta = std::min((-uv).dot(n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

Vec3 Vec3::random_in_unit_disk()
{
    while (true) {
        auto p = Vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

double Vec3::length() const
{
    return std::sqrt(length_squared());
}

double Vec3::length_squared() const
{
    return x() * x() +
            y() * y() +
            z() * z();
}

double Vec3::dot(const Vec3 &rhs) const
{
    return x() * rhs.x() +
            y() * rhs.y() +
            z() * rhs.z();
}

Vec3 Vec3::cross(const Vec3 &rhs) const
{
    return {y() * rhs.z() - z() * rhs.y(),
                z() * rhs.x() - x() * rhs.z(),
                x() * rhs.y() - y() * rhs.x()};
}

Vec3 Vec3::unit_vector() const
{
    return *this / length();
}

bool Vec3::near_zero() const
{
    const double s = 1e-8;
    return (std::fabs(x()) < s) &&
            (std::fabs(y()) < s) &&
            (std::fabs(z()) < s);
}

Vec3 Vec3::operator-() const
{
    return Vec3(-x(), -y(), -z());
}

double Vec3::operator[](size_t index) const
{
    return (*const_cast<Vec3*>(this))[index];
}

double& Vec3::operator[](size_t index)
{
    switch(index) {
    case 0:
        return this->_x;
    case 1:
        return this->_y;
    case 2:
        return this->_z;
    default:
        throw std::out_of_range("Vec index out of range");
    }
}

Vec3& Vec3::operator+=(const Vec3 &rhs)
{
    _x += rhs.x();
    _y += rhs.y();
    _z += rhs.z();

    return *this;
}

Vec3& Vec3::operator*=(double rhs)
{
    _x *= rhs;
    _y *= rhs;
    _z *= rhs;

    return *this;
}

Vec3& Vec3::operator/=(double rhs)
{
    return *this *= 1./rhs;
}

Vec3 Vec3::operator+(const Vec3 &rhs) const
{
    return Vec3(x() + rhs.x(),
                y() + rhs.y(),
                z() + rhs.z());
}

Vec3 Vec3::operator-(const Vec3 &rhs) const
{
    return Vec3(x() - rhs.x(),
                y() - rhs.y(),
                z() - rhs.z());
}

Vec3 Vec3::operator*(const Vec3 &rhs) const
{
    return Vec3(x() * rhs.x(),
                y() * rhs.y(),
                z() * rhs.z());
}

Vec3 Vec3::operator*(double rhs) const
{
    return Vec3(x() * rhs,
                y() * rhs,
                z() * rhs);
}

Vec3 Vec3::operator/(double rhs) const
{
    return Vec3(x() / rhs,
                y() / rhs,
                z() / rhs);
}

Vec3 operator*(double lhs, const Vec3 &rhs)
{
    return rhs * lhs;
}

std::ostream &operator<<(std::ostream &s, const Vec3 &rhs)
{
    return s << rhs.x() << ' ' << rhs.y() << ' ' << rhs.z();
}
