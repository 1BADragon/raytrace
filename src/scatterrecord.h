#ifndef SCATTERRECORD_H
#define SCATTERRECORD_H

#include <memory>
#include <pdf.h>
#include <ray.h>
#include <color.h>

struct ScatterRecord {
    Ray specular_ray;
    bool is_specular;
    Color attenuation;
    std::shared_ptr<Pdf> pdf_ptr;
};

#endif // SCATTERRECORD_H
