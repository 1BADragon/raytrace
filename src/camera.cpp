#include <common.h>
#include <camera.h>

Camera::Camera(const Point3 &lookfrom, const Point3 &lookat, const Vec3 &vup, double vfov, double aspect_ratio, double aperture, double focus_dist, double _time0, double _time1)
{
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2.);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    auto w = (lookfrom - lookat).unit_vector();
    auto u = vup.cross(w).unit_vector();
    auto v = w.cross(u);

    auto tmp_hor = focus_dist * viewport_width * u;
    auto tmp_ver = focus_dist * viewport_height * v;
    auto tmp_llc = lookfrom - tmp_hor / 2. - tmp_ver / 2. - focus_dist * w;

    this->origin = lookfrom;
    this->horizontal = tmp_hor;
    this->vertical = tmp_ver;
    this->lower_left_corner = tmp_llc;
    this->u = u;
    this->v = v;
    this->w = w;
    this->lens_radius = aperture / 2.;
    this->time0 = _time0;
    this->time1 = _time1;
}

Ray Camera::get_ray(double s, double t) const
{
    auto rd = this->lens_radius * Vec3::random_in_unit_disk();
    auto offset = this->u * rd.x() + this->v * rd.y();

    return Ray(this->origin + offset,
               this->lower_left_corner + s * this->horizontal + t *
                    this->vertical - this->origin - offset,
                    random_double(time0, time1));
}
