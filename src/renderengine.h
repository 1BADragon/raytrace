#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <QObject>

class RenderEngine : public QObject
{
    Q_OBJECT
public:
    explicit RenderEngine(QObject *parent = nullptr);
    virtual ~RenderEngine() override;

private:
    size_t n_workers;

};

#endif // RENDERENGINE_H
