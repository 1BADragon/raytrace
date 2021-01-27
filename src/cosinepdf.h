#ifndef COSINEPDF_H
#define COSINEPDF_H

#include <pdf.h>
#include <onb.h>

class CosinePdf : public Pdf
{
public:
    CosinePdf(const Vec3 &w);

    virtual double value(const Vec3 &direction) const override;

    virtual Vec3 generate() const override;

private:
    Onb uvw;
};

#endif // COSINEPDF_H
