#ifndef MAT3_H
#define MAT3_H

#include <array>
#include <vec3.h>

class Mat3
{
public:
    Mat3(): _data() {}
    Mat3(const std::array<Vec3, 3> data): _data(data) {}

    Mat3 cross(const Mat3 &rhs) const;
    Vec3 cross(const Vec3 &rhs) const;

    const Vec3& operator[](size_t index) const {
        return _data[index];
    }

    Vec3& operator[](size_t index) {
        return _data[index];
    }

private:
    /*
     * _data is organized so the array verticies are
     * the x axis and the Vec3 is the Y;
     *
     * so _data[1][2] is column 1 row 2
     */

    std::array<Vec3, 3> _data;
};

#endif // MAT3_H
