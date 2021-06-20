#include <recipe.h>
#include <scene.h>
#include <hittables.h>
#include <textures.h>
#include <materials.h>

static void build_hut(Scene *scene);
static std::shared_ptr<Hittable> build_flower();
static Color from_rgb(int r, int g, int b);
static std::shared_ptr<Hittable> build_fence_segment();
static void build_fence(Scene *scene);

static void config_scene(Scene * s)
{
    s->set_aspect_ratio(16. / 9.);
    s->set_image_width(1200);
    //s->set_background(Color(.4, .4, .7));
    s->set_background(Color(.2, .2, .01));
    s->set_max_depth(200);
    s->set_samples_per_pixel(300);
}

static void config_camera(Scene *s)
{
    Point3 lookfrom(-700,300,500);
    Point3 lookat(0,25,0);
    Vec3 vup(0,1,0);
    double vfov = 20;
    double aperature = .001;
    double focus_dist = 50;

    auto camera = std::make_shared<Camera>(lookfrom, lookat, vup, vfov, s->aspect_ratio(), aperature, focus_dist);
    s->set_camera(camera);
}

static void build_compas(Scene *scene)
{
    auto red = std::make_shared<Lambertian>(Color(1, 0, 0));
    auto green = std::make_shared<Lambertian>(Color(0, 1, 0));
    auto blue = std::make_shared<Lambertian>(Color(0, 0, 1));

    scene->add_object(std::make_shared<Box>(Point3(100, 0, 100),
                                            Point3(130, 5, 105),
                                            red));
    scene->add_object(std::make_shared<Box>(Point3(100, 0, 100),
                                            Point3(105, 30, 105),
                                            green));
    scene->add_object(std::make_shared<Box>(Point3(100, 0, 100),
                                            Point3(105, 5, 130),
                                            blue));
}

BUILD_SCENE
{
    Scene *scene = new Scene();

    config_scene(scene);
    config_camera(scene);

    auto ground_tex = std::make_shared<NoiseTexture>(67, Color(from_rgb(0x57, 0x38, 0x0c)));
    auto ground_mat = std::make_shared<Lambertian>(ground_tex);
    auto light = std::make_shared<DiffuseLight>(Color(7000, 7000, 7000));

    auto ground = std::make_shared<Box>(Point3(-10000, -15, -10000),
                                        Point3(10000, 0, 10000), ground_mat);
    scene->add_object(ground);

    auto sun = std::make_shared<Sphere>(Point3(-1500, 500, -100), 20, light);
    scene->add_object(sun);
    scene->add_light(sun);

    build_compas(scene);

    auto flower = build_flower();

    for (int i = 0; i < 5; i++) {
        auto v = Vec3::random_in_unit_sphere() * 400;
        std::shared_ptr<Hittable> new_flower = std::make_shared<Rotate>(flower, 0, random_double() * 360, 0);
        new_flower = std::make_shared<Translate>(new_flower, Vec3(v.x(), 0, v.y()));
        scene->add_object(new_flower);
    }

    build_hut(scene);
    build_fence(scene);

    return scene;
}

static void build_hut(Scene *scene)
{
    auto hut = std::make_shared<HittableList>();

    auto hut_light_mat = std::make_shared<DiffuseLight>(Color(50, 50, 50));
    auto door_light_mat = std::make_shared<DiffuseLight>(Color(30, 30, 30));
    auto wall_mat = std::make_shared<Lambertian>(Color(.6, .5, .1));
    auto brown = std::make_shared<Lambertian>(Color(.5, .3, .1));
    auto red = std::make_shared<Lambertian>(Color(1, 0, 0));

    auto roof_text = std::make_shared<NoiseTexture>(.3, Color(.2, .2, .2));
    auto roof_mat = std::make_shared<Lambertian>(roof_text);

    auto rot_iron = std::make_shared<Metal>(Color(.1, .1, .1), 1);
    auto mirror_mat = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    auto blue = std::make_shared<Lambertian>(Color(0, 0, 1));

    auto hut_light = std::make_shared<Sphere>(Point3(0, 60, 0), 5, hut_light_mat);
    scene->add_object(hut_light);
    scene->add_light(hut_light);

    hut->add(std::make_shared<Box>(Point3(-30, 0, -30),
                                   Point3(-28, 72, 30),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(30, 0, -30),
                                   Point3(28, 72, 30),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(-30, 0, -30),
                                   Point3(28, 72, -28),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(-30, 70, -30),
                                   Point3(30, 72, 30),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(-30, 40, 28),
                                   Point3(30, 72, 30),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(-30, 0, 28),
                                   Point3(-20, 40, 30),
                                   wall_mat));
    // door
    hut->add(std::make_shared<Box>(Point3(-20, 0, 28),
                                   Point3(0, 40, 30),
                                   brown));
    // window frame
    hut->add(std::make_shared<Box>(Point3(27, 0, 28),
                                   Point3(30, 40, 30),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(0, 0, 28),
                                   Point3(3, 40, 30),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(3, 30, 28),
                                   Point3(27, 40, 30),
                                   wall_mat));
    hut->add(std::make_shared<Box>(Point3(3, 0, 28),
                                   Point3(27, 10, 30),
                                   wall_mat));

    //window
    auto glass = std::make_shared<Dielectric>(1);
    hut->add(std::make_shared<Box>(Point3(3, 10, 28),
                                   Point3(27, 30, 30),
                                   glass));

    // door light
    hut->add(std::make_shared<Box>(Point3(-12, 41, 30),
                                   Point3(-9, 46, 36),
                                   rot_iron));
    auto door_light = std::make_shared<Sphere>(Point3(-9, 46, 36),
                                               1, door_light_mat);
    hut->add(door_light);
    scene->add_light(door_light);

    // roof
    auto roof = std::make_shared<Box>(Point3(-30, 0, -30),
                                      Point3(25, 2, 30),
                                      roof_mat);

    std::shared_ptr<Hittable> roof1 = std::make_shared<Rotate>(roof, 40, 180, 0);
    roof1 = std::make_shared<Translate>(roof1, Vec3(-25, 80, 0));
    hut->add(roof1);

    std::shared_ptr<Hittable> roof2 = std::make_shared<Rotate>(roof, 40, 0, 0);
    roof2 = std::make_shared<Translate>(roof2, Vec3(25, 80, 0));
    hut->add(roof2);

    // mirror
    hut->add(std::make_shared<Box>(Point3(26, 0, 0),
                                   Point3(28, 40, 25),
                                   mirror_mat));

    // occupent
    hut->add(std::make_shared<Sphere>(Point3(-15, 5, -5), 5, blue));

    scene->add_object(hut);
}

static std::shared_ptr<Hittable> build_flower()
{
    auto flower = std::make_shared<HittableList>();
    auto peddels = std::make_shared<HittableList>();

    auto stem_mat = std::make_shared<Lambertian>(from_rgb(58, 95, 11));
    auto yellow = std::make_shared<Lambertian>(from_rgb(232, 222, 4));
    auto white_txt = std::make_shared<SolidColor>(Color(.8, .8, .8));
    auto white = std::make_shared<Lambertian>(white_txt);

    auto center_pillar = std::make_shared<Box>(Point3(-.5, 0, -.5), Point3(.5, 5, .5), stem_mat);
    flower->add(center_pillar);

    std::shared_ptr<Hittable> top = std::make_shared<Sphere>(Point3(0, 6, 0), 1, white);
    top = std::make_shared<ConstantMedium>(top, .3, white_txt);
    flower->add(top);

    return flower;
}

static void build_fence(Scene *scene)
{
    auto seg = build_fence_segment();
    std::shared_ptr<Hittable> f;

    for (int i = 0; i < 12; i += 1) {
        scene->add_object(std::make_shared<Translate>(seg, Vec3(-210, 0, -210. + i * 35.)));
    }

    f = std::make_shared<Rotate>(seg, 0, 90., 0);
    for (int i = 0; i < 12; i += 1) {
        scene->add_object(std::make_shared<Translate>(f, Vec3(-210. + (i + 1) * 35., 0, -210)));
    }

    f = std::make_shared<Rotate>(seg, 0, 180., 0);
    for (int i = 0; i < 12; i += 1) {
        scene->add_object(std::make_shared<Translate>(f, Vec3(210, 0, 210. - i * 35.)));
    }

    f = std::make_shared<Rotate>(seg, 0, 270., 0);
    for (int i = 0; i < 12; i += 1) {
        scene->add_object(std::make_shared<Translate>(f, Vec3(210. - (i + 1) * 35., 0, 210)));
    }
}

static std::shared_ptr<Hittable> build_fence_segment()
{
    auto brown = std::make_shared<Lambertian>(from_rgb(0x85, 0x5E, 0x42));
    auto fence = std::make_shared<HittableList>();

    fence->add(std::make_shared<Box>(Point3(-2, 0, -2),
                                     Point3(2, 20, 2),
                                     brown));
    fence->add(std::make_shared<Box>(Point3(-1, 7, 0),
                                     Point3(1, 9, 35),
                                     brown));
    fence->add(std::make_shared<Box>(Point3(-1, 16, 0),
                                     Point3(1, 18, 35),
                                     brown));

    return fence;
}

static Color from_rgb(int r, int g, int b)
{
    return Color(static_cast<double>(r) / 255,
                 static_cast<double>(g) / 255,
                 static_cast<double>(b) / 255);
}
