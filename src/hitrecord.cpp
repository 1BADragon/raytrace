#include <ray.h>
#include <hitrecord.h>

HitRecord::HitRecord() :
    p(), normal(), mat(nullptr), t(0), front_face(false)
{

}

void HitRecord::set_face_normal(const Ray &r, const Vec3 &outward_normal)
{
    front_face = r.direction().dot(outward_normal) < 0.;

    if (front_face) {
        normal = outward_normal;
    } else {
        normal = -outward_normal;
    }
}
