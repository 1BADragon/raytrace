#ifndef IMAGE_H
#define IMAGE_H

#include <mutex>
#include <vector>

#include <color.h>

class Image
{
public:
    Image(int width, int height);

    void update_at(int x, int y, const Color &val);

    Color color_at(int _x, int _y);

    int width() const;
    int height() const;

    void lock();
    void unlock();

private:
    struct ColorData {
        Color c;
        size_t count;

        ColorData() : c(0, 0, 0), count(0){ }
        void add_point(const Color &c)
        {
            this->c += c;
            this->count++;
        }
    };

    std::recursive_mutex _lock;
    std::vector<std::vector<ColorData>> _data;
};

#endif // IMAGE_H
