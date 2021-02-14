#include <recipe.h>
#include <scene.h>
#include <hittables.h>
#include <textures.h>
#include <materials.h>

static void config_scene(Scene * s)
{
    s->set_aspect_ratio(16. / 9.);
    s->set_image_width(1000);
    s->set_background(Color(.4, .4, .7));
    s->set_max_depth(50);
    s->set_samples_per_pixel(50);
}

static void config_camera(Scene *s)
{
    Point3 lookfrom(-500,70,0);
    Point3 lookat(0,0,0);
    Vec3 vup(0,1,0);
    double vfov = 20;
    double aperature = .1;
    double focus_dist = 50;

    auto camera = std::make_shared<Camera>(lookfrom, lookat, vup, vfov, s->aspect_ratio(), aperature, focus_dist);
    s->set_camera(camera);
}

BUILD_SCENE
{
    Scene *scene = new Scene();

    config_scene(scene);
    config_camera(scene);

    auto ground_mat = std::make_shared<Lambertian>(Color(.4, .6, .1));
    auto red = std::make_shared<Lambertian>(Color(1, .1, .1));

    auto ground = std::make_shared<Sphere>(Point3(0, -10000, 0), 10000, ground_mat);

    scene->add_object(ground);

    auto sun = std::make_shared<Sphere>(Point3(-100, 100, -100), 50, nullptr);
    scene->add_light(sun);

    auto center_pillar = std::make_shared<Box>(Point3(-1, 50, -1), Point3(1, 100, 1), red);

    scene->add_object(center_pillar);

    for (int i = 0; i < 180; i+=30) {
        auto rotate = std::make_shared<Rotate>(center_pillar, 0, 0, i);
        scene->add_object(rotate);
    }

    return scene;
}
