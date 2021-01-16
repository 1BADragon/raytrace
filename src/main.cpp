#include <iostream>

#include <vec3.h>
#include <color.h>
#include <ray.h>
#include <common.h>
#include <hittable.h>
#include <hitrecord.h>
#include <hittablelist.h>
#include <sphere.h>
#include <camera.h>
#include <materials.h>
#include <textures.h>

// Some general constants
constexpr double ASPECT_RATIO = 16.0 / 9.0;
constexpr int IMAGE_WIDTH = 1200;
constexpr int IMAGE_HEIGHT = IMAGE_WIDTH / ASPECT_RATIO;
constexpr int SAMPLES_PER_PIXEL = 50;
constexpr int MAX_DEPTH = 50;

static Color ray_color(const Ray &r, std::shared_ptr<Hittable> world, int depth)
{
    if (depth <= 0) {
        return Color();
    }

    auto rec = HitRecord();

    if (world->hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat) {
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, world, depth-1);
            }
        } else {
            return Color();
        }
    }

    auto unit_direction = r.direction().unit_vector();
    auto t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

static std::shared_ptr<HittableList> random_scene() {
    auto world = std::make_shared<HittableList>();

    auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(.9, .9, .9));
    world->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

    //    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    //    world->add(std::make_shared<Sphere>(Point3(0., -1000., 0.), 1000., ground_material));

    for (int a = -11; a <= 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto choose_mat = random_double();
            auto center = Point3(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4., 0.2, 0.)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    auto sphere_mat = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
                    world->add(std::make_shared<Sphere>(center, center2, 0.0, 1.0, 0.2, sphere_mat));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random_ranged(0.5, 1.0);
                    auto fuzz = random_double(0., 0.5);
                    auto sphere_mat = std::make_shared<Metal>(albedo, fuzz);
                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
                } else {
                    // glass
                    auto sphere_mat = std::make_shared<Dielectric>(1.5);
                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world->add(std::make_shared<Sphere>(Point3(0.0, 1.0, 0.0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world->add(std::make_shared<Sphere>(Point3(-4.0, 1.0, 0.0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world->add(std::make_shared<Sphere>(Point3(4.0, 1.0, 0.0), 1.0, material3));

    return world;
}

static std::shared_ptr<HittableList> two_spheres()
{
    auto objects = std::make_shared<HittableList>();

    auto checker = std::make_shared<CheckerTexture>(Color(.2, .3, .1),
                                                    Color(.9, .9, .9));

    auto lambertian = std::make_shared<Lambertian>(checker);

    objects->add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, lambertian));
    objects->add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, lambertian));

    return objects;
}

static std::shared_ptr<HittableList> two_perlin_spheres()
{
    auto objects = std::make_shared<HittableList>();

    auto pertext = std::make_shared<NoiseTexture>(4);
    auto lambertian = std::make_shared<Lambertian>(pertext);

    objects->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, lambertian));
    objects->add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, lambertian));

    return objects;
}

int main(void) {

    // World
    std::shared_ptr<HittableList> world;

    Point3 lookfrom;
    Point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    switch(0) {
    case 1:
        world = random_scene();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break;

    case 2:
        world = two_spheres();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;

    case 3:
    default:
        world = two_perlin_spheres();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;
    }

    // Camera
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;

    Camera camera(lookfrom, lookat, vup, vfov, ASPECT_RATIO, aperture, dist_to_focus, 0.0, 1.0);

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for (int j = IMAGE_HEIGHT-1; j >= 0; --j) {
        std::clog << "\rScanlines remaning: " << j << ' ' << std::flush;
        for (int i = 0; i < IMAGE_WIDTH; ++i) {
            Color pixel_color;
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
                auto u = (i + random_double()) / (IMAGE_WIDTH - 1);
                auto v = (j + random_double()) / (IMAGE_HEIGHT - 1);
                Ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world, MAX_DEPTH);
            }
            write_color(std::cout, pixel_color, SAMPLES_PER_PIXEL);
        }
    }

    std::clog << "\nDone.\n";
}
