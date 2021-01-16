#ifndef HITRECORD_H
#define HITRECORD_H
#include <memory>
#include <vec3.h>

class Material;
class Ray;

class HitRecord
{
public:
    HitRecord();

    void set_face_normal(const Ray &r, const Vec3 &outward_normal);

    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    double u;
    double v;
    bool front_face;
};

#endif // HITRECORD_H
