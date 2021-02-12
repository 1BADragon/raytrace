#include <recipe.h>
#include <vec3.h>
#include <hittables.h>
#include <textures.h>
#include <materials.h>

BUILD_SCENE
{
    Scene * scene = new Scene();

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int samples_per_pixel = 20;
    const int max_depth = 50;

    scene->set_aspect_ratio(aspect_ratio);
    scene->set_image_width(image_width);
    scene->set_samples_per_pixel(samples_per_pixel);
    scene->set_max_depth(max_depth);
    scene->set_background(Color(0.5, 0.7, 1.0));

    Point3 lookfrom(13,2,3);
    Point3 lookat(0,0,0);
    Vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    auto camera = std::make_shared<Camera>(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 0.1);
    scene->set_camera(camera);

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    scene->world()->add(std::make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    scene->world()->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random_ranged(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene->world()->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    scene->world()->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    scene->world()->add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene->world()->add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene->world()->add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return scene;
}