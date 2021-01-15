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

// Some general constants
constexpr double ASPECT_RATIO = 3.0 / 2.0;
constexpr int IMAGE_WIDTH = 1200;
constexpr int IMAGE_HEIGHT = IMAGE_WIDTH / ASPECT_RATIO;
constexpr int SAMPLES_PER_PIXEL = 2;
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

static std::shared_ptr<Hittable> random_scene() {
    auto world = std::make_shared<HittableList>();

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world->add(std::make_shared<Sphere>(Point3(0., -1000., 0.), 1000., ground_material));

    for (int a = -11; a <= 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto choose_mat = random_double();
            auto center = Point3(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4., 0.2, 0.)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    auto sphere_mat = std::make_shared<Lambertian>(albedo);
                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
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

int main(void) {

    // World
    auto world = random_scene();

    // Camera
    Point3 lookfrom(13., 2., 3.);
    Point3 lookat(0., 0., 0.);
    Vec3 vup(0., 1., 0.);
    double dist_to_focus = 10.;
    double aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 20., ASPECT_RATIO, aperture, dist_to_focus);

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
