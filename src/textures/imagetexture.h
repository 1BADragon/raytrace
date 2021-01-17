#ifndef IMAGETEXTURE_H
#define IMAGETEXTURE_H

#include <iostream>
#include <memory>
#include <string>

#include <common.h>
#include <stb_image.h>
#include <textures/texture.h>
#include <textures/perlin.h>

class ImageTexture : public Texture
{
public:
    static constexpr int bytes_per_pixel = 3;

    ImageTexture() : Texture(),
        data(), width(0), height(0), bytes_per_scanline(0) {}

    ImageTexture(const std::string &filename);

    virtual Color value(double u, double v, const Point3 &p) const override;

private:
    std::unique_ptr<uint8_t[]> data;
    int width;
    int height;
    int bytes_per_scanline;
};

#endif
