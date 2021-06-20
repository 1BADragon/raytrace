#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <memory>
#include <image.h>

class SceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWidget(std::shared_ptr<Image> i, QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *event) override;

signals:

private:
    std::shared_ptr<Image> _image;
};

#endif // SCENEWIDGET_H
