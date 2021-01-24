#include <iostream>
#include <mutex>
#include <thread>
#include <optional>
#include <atomic>
#include <future>
#include <assert.h>

#include <vec3.h>
#include <color.h>
#include <ray.h>
#include <common.h>
#include <hittables.h>
#include <hitrecord.h>
#include <camera.h>
#include <materials.h>
#include <textures.h>
#include <scene.h>
#include <jsonparser.h>

// Some general constants
constexpr int N_THREADS = 4;


static Color ray_color(const Ray &r, const Color &background,
                       std::shared_ptr<Hittable> world, int depth)
{
    if (depth <= 0) {
        return Color();
    }

    auto rec = HitRecord();

    if (!world->hit(r, 0.001, infinity, rec)) {
        return background;
    }

    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

//static std::shared_ptr<HittableList> random_scene() {
//    auto world = std::make_shared<HittableList>();

//    auto checker = std::make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(.9, .9, .9));
//    world->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

//    //    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
//    //    world->add(std::make_shared<Sphere>(Point3(0., -1000., 0.), 1000., ground_material));

//    for (int a = -11; a <= 11; ++a) {
//        for (int b = -11; b < 11; ++b) {
//            auto choose_mat = random_double();
//            auto center = Point3(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

//            if ((center - Point3(4., 0.2, 0.)).length() > 0.9) {
//                if (choose_mat < 0.8) {
//                    // diffuse
//                    auto albedo = Color::random() * Color::random();
//                    auto sphere_mat = std::make_shared<Lambertian>(albedo);
//                    auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
//                    world->add(std::make_shared<Sphere>(center, center2, 0.0, 1.0, 0.2, sphere_mat));
//                } else if (choose_mat < 0.95) {
//                    // metal
//                    auto albedo = Color::random_ranged(0.5, 1.0);
//                    auto fuzz = random_double(0., 0.5);
//                    auto sphere_mat = std::make_shared<Metal>(albedo, fuzz);
//                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
//                } else {
//                    // glass
//                    auto sphere_mat = std::make_shared<Dielectric>(1.5);
//                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_mat));
//                }
//            }
//        }
//    }

//    auto material1 = std::make_shared<Dielectric>(1.5);
//    world->add(std::make_shared<Sphere>(Point3(0.0, 1.0, 0.0), 1.0, material1));

//    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
//    world->add(std::make_shared<Sphere>(Point3(-4.0, 1.0, 0.0), 1.0, material2));

//    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
//    world->add(std::make_shared<Sphere>(Point3(4.0, 1.0, 0.0), 1.0, material3));

//    return world;
//}

//static std::shared_ptr<HittableList> two_spheres()
//{
//    auto objects = std::make_shared<HittableList>();

//    auto checker = std::make_shared<CheckerTexture>(Color(.2, .3, .1),
//                                                    Color(.9, .9, .9));

//    auto lambertian = std::make_shared<Lambertian>(checker);

//    objects->add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, lambertian));
//    objects->add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, lambertian));

//    return objects;
//}

//static std::shared_ptr<HittableList> two_perlin_spheres()
//{
//    auto objects = std::make_shared<HittableList>();

//    auto pertext = std::make_shared<NoiseTexture>(4);
//    auto lambertian = std::make_shared<Lambertian>(pertext);

//    objects->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, lambertian));
//    objects->add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, lambertian));

//    return objects;
//}

//static std::shared_ptr<HittableList> earth() {
//    auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
//    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
//    auto globe = std::make_shared<Sphere>(Point3(0,0,0), 2, earth_surface);

//    auto scene = std::make_shared<HittableList>();
//    scene->add(globe);

//    return scene;
//}

//static std::shared_ptr<HittableList> simple_light()
//{
//    auto objects = std::make_shared<HittableList>();

//    auto pertext = std::make_shared<NoiseTexture>(4);
//    objects->add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
//    objects->add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

//    auto difflight = std::make_shared<DiffuseLight>(Color(4, 4, 4));
//    objects->add(std::make_shared<XYRect>(3, 5, 1, 3, -2, difflight));

//    return objects;
//}

//static std::shared_ptr<HittableList> cornell_box() {
//    auto objects = std::make_shared<HittableList>();

//    auto red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
//    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
//    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
//    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

//    objects->add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
//    objects->add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
//    objects->add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
//    objects->add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
//    objects->add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
//    objects->add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));


//    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
//    box1 = std::make_shared<RotateY>(box1, 15);
//    box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
//    objects->add(box1);

//    std::shared_ptr<Hittable> box2;
//    box2 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
//    box2 = std::make_shared<RotateY>(box2, -18);
//    box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
//    objects->add(box2);

//    return objects;
//}

//static std::shared_ptr<HittableList> cornell_smoke() {
//    auto objects = std::make_shared<HittableList>();

//    auto red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
//    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
//    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
//    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

//    objects->add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
//    objects->add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
//    objects->add(std::make_shared<XZRect>(113, 443, 127, 432, 554, light));
//    objects->add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
//    objects->add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
//    objects->add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

//    std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0,0,0), Point3(165,330,165), white);
//    box1 = std::make_shared<RotateY>(box1, 15);
//    box1 = std::make_shared<Translate>(box1, Vec3(265,0,295));

//    std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3(0,0,0), Point3(165,165,165), white);
//    box2 = std::make_shared<RotateY>(box2, -18);
//    box2 = std::make_shared<Translate>(box2, Vec3(130,0,65));

//    objects->add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0,0,0)));
//    objects->add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1,1,1)));

//    return objects;
//}

//static std::shared_ptr<HittableList> final_scene() {
//    auto boxes1 = std::make_shared<HittableList>();
//    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

//    const int boxes_per_side = 20;
//    for (int i = 0; i < boxes_per_side; i++) {
//        for (int j = 0; j < boxes_per_side; j++) {
//            auto w = 100.0;
//            auto x0 = -1000.0 + i*w;
//            auto z0 = -1000.0 + j*w;
//            auto y0 = 0.0;
//            auto x1 = x0 + w;
//            auto y1 = random_double(1,101);
//            auto z1 = z0 + w;

//            boxes1->add(std::make_shared<Box>(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
//        }
//    }

//    auto objects = std::make_shared<HittableList>();

//    objects->add(std::make_shared<BvhNode>(boxes1, 0., 1.));

//    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
//    objects->add(std::make_shared<XZRect>(123, 423, 147, 412, 554, light));

//    auto center1 = Point3(400, 400, 200);
//    auto center2 = center1 + Vec3(30,0,0);
//    auto moving_sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
//    objects->add(std::make_shared<Sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

//    objects->add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
//    objects->add(std::make_shared<Sphere>(
//        Point3(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
//    ));

//    auto boundary = std::make_shared<Sphere>(Point3(360,150,145), 70, std::make_shared<Dielectric>(1.5));
//    objects->add(boundary);
//    objects->add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
//    boundary = std::make_shared<Sphere>(Point3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
//    objects->add(std::make_shared<ConstantMedium>(boundary, .0001, Color(1,1,1)));

//    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
//    objects->add(std::make_shared<Sphere>(Point3(400,200,400), 100, emat));
//    auto pertext = std::make_shared<NoiseTexture>(0.1);
//    objects->add(std::make_shared<Sphere>(Point3(220,280,300), 80, std::make_shared<Lambertian>(pertext)));

//    auto boxes2 = std::make_shared<HittableList>();
//    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
//    int ns = 1000;
//    for (int j = 0; j < ns; j++) {
//        boxes2->add(std::make_shared<Sphere>(Point3::random_ranged(0,165), 10, white));
//    }

//    objects->add(std::make_shared<Translate>(
//        std::make_shared<RotateY>(
//            std::make_shared<BvhNode>(boxes2, 0.0, 1.0), 15),
//            Vec3(-100,270,395)
//        )
//    );

//    return objects;
//}

struct WorkerCtx {
    std::shared_ptr<Scene> scene;
    std::thread t;
    std::shared_ptr<std::mutex> res_mut;
    std::shared_ptr<std::vector<std::vector<Color>>> res;
    std::shared_ptr<std::mutex> line_mut;
    std::shared_ptr<int> next_line;
};

static void worker(std::shared_ptr<WorkerCtx> w)
{
    std::vector<Color> row;
    row.resize(w->scene->image_width());
    const int image_width = w->scene->image_width();
    const int image_height = w->scene->image_height();

    for(;;) {
        int j;

        {
            std::lock_guard _(*w->line_mut);
            j = *w->next_line;
            if (j < 0) {
                return;
            }
            (*w->next_line)--;
            std::clog << "\rScanlines remaining: " << j << "   " << std::flush;
        }

        for (int i = 0; i < w->scene->image_width(); ++i) {
            Color pixel_color;
            for (int s = 0; s < w->scene->samples_per_pixel(); ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = w->scene->camera()->get_ray(u, v);
                pixel_color += ray_color(r, w->scene->background(), w->scene->world(), w->scene->max_depth());
            }
            row[i] = pixel_color;
        }

        {
            std::lock_guard _(*w->res_mut);
            w->res->at(image_height - (j + 1)) = row;
        }

    }
}

int main(int argc, char **argv)
{

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <scene file>";
        return 1;
    }

    JsonParser jp;

    auto scene = jp.build_from_file(argv[1]);

    std::cerr << "Image size: " << scene->image_width() << ", "
              << scene->image_height() << std::endl;
    std::cerr << "spp: " << scene->samples_per_pixel() << std::endl;
    std::cerr << "max_depth: " << scene->max_depth() << std::endl;

//    switch(0) {
//    case 1:
//        world = random_scene();
//        background = Color(.7, .8, 1.);
//        lookfrom = Point3(13, 2, 3);
//        lookat = Point3(0, 0, 0);
//        vfov = 20.0;
//        aperture = 0.1;
//        break;

//    case 2:
//        world = two_spheres();
//        background = Color(0.70, 0.80, 1.00);
//        lookfrom = Point3(13, 2, 3);
//        lookat = Point3(0, 0, 0);
//        vfov = 20.0;
//        break;

//    case 3:
//        world = two_perlin_spheres();
//        background = Color(0.70, 0.80, 1.00);
//        lookfrom = Point3(13, 2, 3);
//        lookat = Point3(0, 0, 0);
//        vfov = 20.0;
//        break;

//    case 4:
//        world = earth();
//        background = Color(0.70, 0.80, 1.00);
//        lookfrom = Point3(13, 2, 3);
//        lookat = Point3(0, 0, 0);
//        vfov = 20.0;
//        break;

//    case 5:
//        world = simple_light();
//        background = Color(0, 0, 0);
//        lookfrom = Point3(26, 3, 6);
//        lookat = Point3(0, 2, 0);
//        vfov = 20.0;
//        break;

//    case 6:
//        world = cornell_box();
//        background = Color(0,0,0);
//        lookfrom = Point3(278, 278, -800);
//        lookat = Point3(278, 278, 0);
//        vfov = 40.0;
//        break;

//    case 7:
//        world = cornell_smoke();
//        lookfrom = Point3(278, 278, -800);
//        lookat = Point3(278, 278, 0);
//        vfov = 40.0;
//        break;

//    default:
//    case 8:
//        world = final_scene();
//        background = Color(0,0,0);
//        lookfrom = Point3(478, 278, -600);
//        lookat = Point3(278, 278, 0);
//        vfov = 40.0;
//        break;
//    }
    std::vector<std::shared_ptr<WorkerCtx>> workers;

    auto image = std::make_shared<std::vector<std::vector<Color>>>();
    image->resize(scene->image_height());
    auto res_mut = std::make_shared<std::mutex>();
    auto line_mut = std::make_shared<std::mutex>();
    auto next_line = std::make_shared<int>(scene->image_height() - 1);

    for (int i = 0; i < N_THREADS; ++i) {
        auto worker_ctx = std::make_shared<WorkerCtx>();
        worker_ctx->scene = scene;
        // worker->thread...
        worker_ctx->res_mut = res_mut;
        worker_ctx->res = image;
        worker_ctx->line_mut = line_mut;
        worker_ctx->next_line = next_line;

        worker_ctx->t = std::thread(worker, worker_ctx);

        workers.push_back(worker_ctx);
    }

    // wait for workers
    for (auto &worker : workers) {
        worker->t.join();
    }

    std::cout << "P3\n" << scene->image_width() << ' ' << scene->image_height() << "\n255\n";

    for (auto &scanline : *image) {
        for (auto &pixel : scanline) {
            write_color(std::cout, pixel, scene->samples_per_pixel());
        }
    }

    std::clog << "\nDone.\n";
}
