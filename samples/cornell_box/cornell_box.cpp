#include <recipe.h>
#include <textures.h>
#include <materials.h>
#include <hittables.h>

BUILD_SCENE {
    Scene *scene = new Scene();

    scene->set_aspect_ratio(1.0 / 1.0);
    scene->set_image_width(600);
    scene->set_samples_per_pixel(10);
    scene->set_max_depth(50);
    scene->set_background(Color(0.0, 0.0, 0.0));

    Point3 lookfrom(278, 278, -800);
    Point3 lookat(278, 278, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto time0 = 0.0;
    auto time1 = 1.0;

    auto camera = std::make_shared<Camera>(lookfrom, lookat, vup, vfov, scene->aspect_ratio(), aperture, dist_to_focus, time0, time1);

    scene->set_camera(camera);

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    scene->world()->add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
    scene->world()->add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    scene->world()->add(std::make_shared<FlipFace>(std::make_shared<XZRect>(213, 343, 227, 332, 554, light)));
    scene->world()->add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    scene->world()->add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    scene->world()->add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

    scene->lights()->add(std::make_shared<FlipFace>(std::make_shared<XZRect>(213, 343, 227, 332, 554, std::shared_ptr<Material>())));

    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0,0,0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
    scene->world()->add(box1);

    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3(0,0,0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
    scene->world()->add(box2);

    return scene;
}
