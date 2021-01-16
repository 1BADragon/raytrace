#ifndef CHECKERTEXTURE_H
#define CHECKERTEXTURE_H

#include <memory>
#include <texture.h>
#include <solidcolor.h>

class CheckerTexture : public Texture
{
public:
    CheckerTexture() {}

    CheckerTexture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd) : even(_even), odd(_odd) {}

    CheckerTexture(Color c1, Color c2)
                : even(std::make_shared<SolidColor>(c1)) , odd(std::make_shared<SolidColor>(c2)) {}

    virtual Color value(double u, double v, const Point3 &p) const override;

private:
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;
};

#endif
