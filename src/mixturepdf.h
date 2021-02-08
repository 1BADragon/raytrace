#ifndef MIXTUREPDF_H
#define MIXTUREPDF_H

#include <memory>
#include <pdf.h>
#include <common.h>

class MixturePdf : public Pdf {
public:
    MixturePdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1)
    {
        p[0] = p0;
        p[1] = p1;
    }

    virtual double value(const Vec3& direction) const override
    {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    virtual Vec3 generate() const override
    {
        if (random_double() < 0.5) {
            return p[0]->generate();
        } else {
            return p[1]->generate();
        }
    }

public:
    std::shared_ptr<Pdf> p[2];
};

#endif // MIXTUREPDF_H
