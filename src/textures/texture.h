#ifndef TEXTURE_H
#define TEXTURE_H

#include <common.h>
#include <color.h>

class Texture {
public:
    virtual Color value(double u, double v, const Point3& p) const = 0;
};

#endif
