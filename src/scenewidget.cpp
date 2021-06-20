#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <scenewidget.h>
#include <color.h>

SceneWidget::SceneWidget(std::shared_ptr<Image> i, QWidget *parent) : QWidget(parent)
{
    _image = i;
}

void SceneWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    _image->lock();

    QImage image(_image->width(), _image->height(), QImage::Format_RGB32);

    for (int j = 0; j < _image->height(); ++j) {
        for (int i = 0; i < _image->width(); ++i) {
            auto color = _image->color_at(i, j);

            image.setPixelColor(i, j, QColor(color.x(), color.y(), color.z()));
        }
    }

    _image->unlock();

    painter.drawImage(QRectF(0, 0, width(), height()), image);
    event->accept();
}
