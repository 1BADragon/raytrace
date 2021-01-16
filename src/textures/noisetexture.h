#ifndef NOISETEXTURE_H
#define NOISETEXTURE_H

#include <textures/perlin.h>
#include <textures/texture.h>

class NoiseTexture : public Texture
{
public:
    NoiseTexture() {}
    NoiseTexture(double sc) : scale(sc) {}

    virtual Color value(double u, double v, const Point3 &p) const override
    {
        (void) u;
        (void) v;
        return Color(1, 1, 1) * 0.5 *
                (1 + std::sin(scale * p.z() + 10 * noise.turb(p)));
    }

private:
    Perlin noise;
    double scale;
};

#endif
