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
#include <cosinepdf.h>
#include <hittablepdf.h>

// Some general constants
constexpr int N_THREADS = 4;


static Color ray_color(const Ray &r, const Color &background,
                       std::shared_ptr<Hittable> world, std::shared_ptr<Hittable> lights, int depth)
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
    Color emitted = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);
    double pdf_val;
    Color albedo;

    if (!rec.mat->scatter(r, rec, albedo, scattered, pdf_val)) {
        return emitted;
    }
    HittablePdf light_pdf(lights, rec.p);
    scattered = Ray(rec.p, light_pdf.generate(), r.time());
    pdf_val = light_pdf.value(scattered.direction());

    return emitted +
            albedo * rec.mat->scattering_pdf(r, rec, scattered) *
            ray_color(scattered, background, world, lights, depth-1) / pdf_val;
}

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
                pixel_color += ray_color(r, w->scene->background(), w->scene->world(),
                                         w->scene->lights(), w->scene->max_depth());
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
    std::cerr << "Light count: " << scene->lights()->n_objects() << std::endl;
    std::cerr << "Object count: " << scene->world()->n_objects() << std::endl;

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
