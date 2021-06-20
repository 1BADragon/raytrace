#include <QApplication>
#include <QStatusBar>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <assert.h>
#include <dlfcn.h>
#include <signal.h>

#include <scene.h>
#include <color.h>
#include <jsonparser.h>

#include <mainwindow.h>

static std::atomic<bool> should_run;

static void sighandle(int sig)
{
    if (sig == SIGINT) {
        should_run.store(false);
    }
}

void MainWindow::worker(std::shared_ptr<MainWindow::WorkerCtx> w)
{
    const int image_width = w->scene->image_width();
    const int image_height = w->scene->image_height();

    while(should_run.load()) {
        int j;

        {
            std::lock_guard _(*w->line_mut);
            j = *w->next_line;
            if (j < 0) {
                return;
            }
            (*w->next_line)++;
            if (*w->next_line >= w->scene->image_height()) {
                *w->next_line = 0;
                (*w->pass)++;
                statusBar()->showMessage(QString("Pass %1").arg(*w->pass));
                central->update();
            }
        }

        for (int i = 0; i < w->scene->image_width(); ++i) {
            Color pixel_color;
            auto u = (i + random_double()) / (image_width - 1);
            auto v = (j + random_double()) / (image_height - 1);
            Ray r = w->scene->camera()->get_ray(u, v);
            pixel_color += ray_color(r, w->scene->background(), w->scene->world(),
                                     w->scene->lights(), w->scene->max_depth());

            w->image->update_at(i, image_height - (j + 1), pixel_color);
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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    n_workers = std::thread::hardware_concurrency();

    std::vector<std::string> args;

    for (auto &arg : QApplication::arguments()) {
        args.push_back(arg.toStdString());
    }

    scene = load_scene_from_args(args);

    if (!scene) {
        throw std::runtime_error("Unable to load scene");
    }

    std::cerr << "Worker count: " << n_workers << std::endl;
    std::cerr << "Image size: " << scene->image_width() << ", "
              << scene->image_height() << std::endl;
    std::cerr << "spp: " << scene->samples_per_pixel() << std::endl;
    std::cerr << "max_depth: " << scene->max_depth() << std::endl;
    std::cerr << "Light count: " << scene->lights()->n_objects() << std::endl;
    std::cerr << "Object count: " << scene->world()->n_children() << std::endl;

    should_run.store(true);
    signal(SIGINT, sighandle);

    image = std::make_shared<Image>(scene->image_width(), scene->image_height());

    statusBar()->showMessage("Building workers");

    auto res_mut = std::make_shared<std::mutex>();
    auto line_mut = std::make_shared<std::mutex>();
    auto next_line = std::make_shared<int>(scene->image_height() - 1);

    central = new SceneWidget(image, this);
    this->setCentralWidget(central);

    central->setMinimumSize(scene->image_width(), scene->image_height());

    auto pass = std::make_shared<int>(0);

    for (size_t i = 0; i < n_workers; ++i) {
        auto worker_ctx = std::make_shared<WorkerCtx>();
        worker_ctx->scene = scene;
        // worker->thread...
        worker_ctx->res_mut = res_mut;
        worker_ctx->image = image;
        worker_ctx->line_mut = line_mut;
        worker_ctx->next_line = next_line;
        worker_ctx->pass = pass;

        worker_ctx->t = std::thread(&MainWindow::worker, this, worker_ctx);

        workers.push_back(worker_ctx);
    }
}

MainWindow::~MainWindow()
{
    should_run.store(false);
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

    image->lock();

    for (int j = 0; j < image->height(); j++) {
        for (int i = 0; i < image->width(); i++) {
            write_color(outf, image->color_at(i, j));
        }
    }

    image->unlock();

    outf.close();

    std::clog << "\nDone.\n";
}
