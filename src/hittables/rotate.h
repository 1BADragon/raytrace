#ifndef ROTATEY_H
#define ROTATEY_H

#include <memory>
#include <hittables/hittable.h>
#include <mat3.h>

class Rotate : public Hittable
{
public:
    Rotate(std::shared_ptr<Hittable> p, double x, double y, double z);

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

    virtual int n_children() const override;

private:
    std::shared_ptr<Hittable> ptr;
    Mat3 _transform;
    Mat3 _inv_transform;
    bool hasbox;
    Aabb bbox;

    static Mat3 build_transform_mat(double a, double b, double g);
};

#endif
