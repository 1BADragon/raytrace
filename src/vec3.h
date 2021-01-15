#ifndef VEC3_H
#define VEC3_H

#include <cstddef>
#include <iostream>

class Vec3
{
public:
    Vec3();
    Vec3(double x, double y, double z);

    static Vec3 random();
    static Vec3 random_ranged(double min, double max);
    static Vec3 random_unit_sphere();
    static Vec3 random_in_hemi(const Vec3 &normal);
    static Vec3 random_unit_vec();
    static Vec3 reflect(const Vec3 &v, const Vec3 &n);
    static Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat);
    static Vec3 random_in_unit_disk();

    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    double length() const;
    double length_squared() const;
    double dot(const Vec3 &rhs) const;
    Vec3 cross(const Vec3 &rhs) const;

    Vec3 unit_vector() const;
    bool near_zero() const;

    Vec3 operator-() const;
    double operator[](size_t index) const;
    double &operator[](size_t index);
    Vec3& operator+=(const Vec3 &rhs);
    Vec3& operator*=(double rhs);
    Vec3& operator/=(double rhs);

    Vec3 operator+(const Vec3 &rhs) const;
    Vec3 operator-(const Vec3 &rhs) const;
    Vec3 operator*(const Vec3 &rhs) const;
    Vec3 operator*(double rhs) const;
    Vec3 operator/(double rhs) const;

private:
    double _x;
    double _y;
    double _z;
};

Vec3 operator*(double lhs, const Vec3 &rhs);

std::ostream &operator<<(std::ostream& s, const Vec3 &rhs);

using Point3 = Vec3;

#endif // VEC3_H
