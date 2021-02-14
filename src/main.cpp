#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <assert.h>
#include <dlfcn.h>

#include <scene.h>
#include <color.h>
#include <jsonparser.h>

// Some general constants
constexpr int N_THREADS = 4;

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

static std::shared_ptr<Scene> load_scene_from_args(std::vector<std::string> &args)
{
    // TODO: Implement an argument parser
    if (args.size() < 3) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return nullptr;
    }

    std::string type = args[1];
    std::string file = args[2];

    if (type == "--json") {
        JsonParser j;
        return j.build_from_file(file);
    } else if (type == "--so") {
        void *handle = dlopen(file.c_str(), RTLD_LOCAL | RTLD_NOW | RTLD_NODELETE);
        if (!handle) {
            std::cerr << "Error opening " << file << ": " << dlerror() << std::endl;
            return nullptr;
        }
        auto build_scene = reinterpret_cast<Scene*(*)()>(dlsym(handle, "build_scene"));
        if (build_scene == nullptr) {
            std::cerr << "Unable to locate build_scene()" << std::endl;
            return nullptr;
        }
        std::shared_ptr<Scene> scene(build_scene());
        dlclose(handle);
        return scene;
    }

    std::cerr << "Unknown option: " << type << std::endl;
    return nullptr;
}

int main(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv+argc);

    auto scene = load_scene_from_args(args);

    if (!scene) {
        return 1;
    }

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

#if 0
    auto &outf = std::cout;
#else
    auto outf = std::ofstream("image.ppm");
#endif

    outf << "P3\n" << scene->image_width() << ' ' << scene->image_height() << "\n255\n";

    for (auto &scanline : *image) {
        for (auto &pixel : scanline) {
            write_color(outf, pixel, scene->samples_per_pixel());
        }
    }

    outf.close();

    system("display image.ppm");

    std::clog << "\nDone.\n";
}
