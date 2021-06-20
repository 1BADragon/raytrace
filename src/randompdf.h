#ifndef RANDOMPDF_H
#define RANDOMPDF_H

#include <pdf.h>

class RandomPdf : public Pdf
{
public:    
    virtual double value(const Vec3 &direction) const override;
    virtual Vec3 generate() const override;
};

#endif // RANDOMPDF_H
