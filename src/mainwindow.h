#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <memory>
#include <mutex>
#include <QMainWindow>

#include <scenewidget.h>
#include <color.h>
#include <scene.h>
#include <image.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:


    struct WorkerCtx {
        std::shared_ptr<Scene> scene;
        std::thread t;
        std::shared_ptr<std::mutex> res_mut;
        std::shared_ptr<Image> image;
        std::shared_ptr<std::mutex> line_mut;
        std::shared_ptr<int> next_line;
        std::shared_ptr<int> pass;
    };
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

signals:


private:
    SceneWidget *central;

    size_t n_workers;
    std::shared_ptr<Scene> scene;
    std::vector<std::shared_ptr<WorkerCtx>> workers;
    std::shared_ptr<Image> image;

    void worker(std::shared_ptr<MainWindow::WorkerCtx> w);

};

#endif // MAINWINDOW_H
