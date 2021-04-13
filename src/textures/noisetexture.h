#ifndef NOISETEXTURE_H
#define NOISETEXTURE_H

#include <textures/perlin.h>
#include <textures/texture.h>

class NoiseTexture : public Texture
{
public:
    NoiseTexture() {}
    NoiseTexture(double sc) : scale(sc), base(1, 1, 1) {}
    NoiseTexture(double sc, Color base) : scale(sc), base(base) {}

    virtual Color value(double u, double v, const Point3 &p) const override
    {
        (void) u;
        (void) v;
        return base * 0.5 *
                (1 + std::sin(scale * p.z() + 10 * noise.turb(p)));
    }

private:
    Perlin noise;
    double scale;
    Color base;
};

#endif
