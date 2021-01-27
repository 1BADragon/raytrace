#ifndef PDF_H
#define PDF_H

#include <vec3.h>

class Pdf {
public:
    virtual ~Pdf() {}

    virtual double value(const Vec3 &direction) const = 0;
    virtual Vec3 generate() const = 0;
};

#endif // PDF_H
