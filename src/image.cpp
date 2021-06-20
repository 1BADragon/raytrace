#include <algorithm>
#include <image.h>

Image::Image(int width, int height)
{
    _data.resize(height);

    for (auto &line : _data) {
        line.resize(width);
    }
}

void Image::update_at(int x, int y, const Color &val)
{
    std::lock_guard _(this->_lock);

    _data.at(y).at(x).add_point(val);
}

Color Image::color_at(int _x, int _y)
{
    std::lock_guard _(this->_lock);

    auto &point = _data.at(_y).at(_x);
    auto pixel_color = point.c;
    auto samples_per_pixel = point.count;

    if (samples_per_pixel == 0) {
        return Color(0, 0, 0);
    }

    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (qIsNaN(r)) r = 0.0;
    if (qIsNaN(g)) g = 0.0;
    if (qIsNaN(b)) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    double scale = 1.0 / static_cast<double>(samples_per_pixel);
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    auto x = static_cast<int>(256 * r);
    auto y = static_cast<int>(256 * g);
    auto z = static_cast<int>(256 * b);

    x = std::clamp(x, 0, 255);
    y = std::clamp(y, 0, 255);
    z = std::clamp(z, 0, 255);

    return Color(x, y, z);
}

int Image::width() const
{
    return _data.at(0).size();
}

int Image::height() const
{
    return _data.size();
}

void Image::lock()
{
    _lock.lock();
}

void Image::unlock()
{
    _lock.unlock();
}
