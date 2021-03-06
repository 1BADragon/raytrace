#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <hittables/hittablelist.h>
#include <hittables/hittable.h>
#include <color.h>
#include <camera.h>

class Scene
{
public:
    Scene() : _world(std::make_shared<HittableList>()),
    _lights(std::make_shared<HittableList>()){}

    void add_object(std::shared_ptr<Hittable> h) { _world->add(h); }
    void add_light(std::shared_ptr<Hittable> l) { _lights->add(l); }

    void set_aspect_ratio(double r) { _aspect_ratio = r; }
    void set_image_width(int v) { _image_width = v; }
    void set_samples_per_pixel(int s) { _samples_per_pixel = s; }
    void set_max_depth(int d) { _max_depth = d; }
    void set_background(Color c) { _background = c; }
    void set_camera(std::shared_ptr<Camera> c) { _camera = c; }

    double aspect_ratio() const { return _aspect_ratio; }
    int image_width() const { return _image_width; }
    int image_height() const { return image_width() / aspect_ratio(); }
    int samples_per_pixel() const { return _samples_per_pixel; }
    int max_depth() const { return _max_depth; }

    std::shared_ptr<HittableList> world() const { return _world; }
    std::shared_ptr<HittableList> lights() const { return _lights; }
    std::shared_ptr<Camera> camera() const { return _camera; }

    Color background() { return _background; }

private:
    double _aspect_ratio;
    int _image_width;
    int _samples_per_pixel;
    int _max_depth;
    Color _background;
    std::shared_ptr<HittableList> _world;
    std::shared_ptr<HittableList> _lights;
    std::shared_ptr<Camera> _camera;
};

#endif // SCENE_H
