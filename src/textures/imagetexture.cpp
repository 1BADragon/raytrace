#include <algorithm>
#include <textures/imagetexture.h>
#include <stb_image.h>

ImageTexture::ImageTexture(const std::string &filename)
{
    auto components_per_pixel = bytes_per_pixel;

    data.reset(stbi_load(filename.c_str(), &width, &height, &components_per_pixel, components_per_pixel));

    if (!data) {
        std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
        width = 0;
        height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

Color ImageTexture::value(double u, double v, const Point3 &p) const
{
    (void)p;
    if (!data) {
        return Color(0, 1, 1);
    }

    u = std::clamp(u, 0., 1.);
    v = 1.0 - std::clamp(v, 0., 1.);

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    if (i >= width) {
        i = width - 1;
    }

    if (j >= width) {
        j = width - 1;
    }

    const auto color_scale = 1.0 / 255.0;
    auto pixel = &(data[j*bytes_per_scanline + i*bytes_per_pixel]);

    return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
}
