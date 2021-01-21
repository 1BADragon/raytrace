#ifndef SCENE_H
#define SCENE_H


#include <memory>
#include <hittables/hittablelist.h>
#include <hittables/hittable.h>

class Scene
{
public:
    Scene() : _world(std::make_shared<HittableList>()) {}

    void add_object(std::shared_ptr<Hittable> h) { _world->add(h); }

    void set_aspect_ratio(double r) { _aspect_ratio = r; }
    void set_image_width(int v) { _image_width = v; }
    void set_samples_per_pixel(int s) { _samples_per_pixel = s; }
    void set_max_depth(int d) { _max_depth = d; }

    double aspect_ratio() const { return _aspect_ratio; }
    int image_width() const { return _image_width; }
    int image_height() const { return image_width() / aspect_ratio(); }
    int samples_per_pixel() const { return _samples_per_pixel; }
    int max_depth() const { return _max_depth; }

    std::shared_ptr<HittableList> world() const { return _world; }

private:
    double _aspect_ratio;
    int _image_width;
    int _samples_per_pixel;
    int _max_depth;
    std::shared_ptr<HittableList> _world;
};


constexpr double ASPECT_RATIO = 1.;
constexpr int IMAGE_WIDTH = 500;
constexpr int IMAGE_HEIGHT = IMAGE_WIDTH / ASPECT_RATIO;
constexpr int SAMPLES_PER_PIXEL = 10000;
constexpr int MAX_DEPTH = 50;

#endif // SCENE_H
