#ifndef HITTABLEPDF_H
#define HITTABLEPDF_H

#include <pdf.h>
#include <hittables/hittable.h>
#include <memory>

class HittablePdf : public Pdf
{
public:
    HittablePdf(std::shared_ptr<Hittable> p, const Point3 &origin);

    virtual double value(const Vec3 &direction) const override;

    virtual Vec3 generate() const override;

private:
    Point3 o;
    std::shared_ptr<Hittable> ptr;
};

#endif // HITTABLEPDF_H
