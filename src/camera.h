#ifndef CAMERA_H
#define CAMERA_H

#include <vec3.h>
#include <ray.h>

class Camera
{
public:
    Camera(const Point3 &lookfrom, const Point3 &lookat, const Vec3 &vup,
           double vfov, double aspect_ratio, double aperture, double focus_dist);

    Ray get_ray(double s, double t) const;

private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u;
    Vec3 v;
    Vec3 w;
    double lens_radius;
};

#endif // CAMERA_H
