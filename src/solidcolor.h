#ifndef SOLIDCOLOR_H
#define SOLIDCOLOR_H

#include <texture.h>

class SolidColor : public Texture {
public:
    SolidColor() {}
    SolidColor(const Color &c): color_value(c) {}

    SolidColor(double red, double green, double blue)
        : SolidColor(Color(red, green, blue)) {}

    virtual Color value(double u, double v, const Point3 &p) const override;

private:
    Color color_value;
};

#endif
