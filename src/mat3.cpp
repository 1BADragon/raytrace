#include "mat3.h"


Mat3 Mat3::cross(const Mat3 &rhs) const
{
    Mat3 result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int u = 0; u < 3; u++) {
                result[i][j] += (*this)[i][u] * rhs[u][j];
            }
        }
    }

    return result;
}

Vec3 Mat3::cross(const Vec3 &rhs) const
{
    Vec3 result;

    for(int i=0; i<3; i++)
    {

        for(int j=0; j<3; j++)
        {
            result[i] += ((*this)[i][j] * rhs[j]);
        }
    }

    return result;
}
