#include <hittables/constantmedium.h>
#include <hitrecord.h>

bool ConstantMedium::hit(const Ray &r, double min, double max, HitRecord &rec) const
{
    constexpr bool enable_debug = false;
    const bool debugging = enable_debug && random_double() < 0.00001;

    HitRecord rec1;
    HitRecord rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1)) {
        return false;
    }

    if (!boundary->hit(r, rec1.t+0.0001, infinity, rec2)) {
        return false;
    }

    if (debugging) {
        std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << "\n";
    }

    if (rec1.t < min) {
        rec1.t = min;
    }

    if (rec2.t > max) {
        rec2.t = max;
    }

    if (rec1.t >= rec2.t) {
        return false;
    }

    if (rec1.t < 0) {
        rec1.t = 0;
    }

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (hit_distance > distance_inside_boundary) {
        return false;
    }

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_distance << '\n'
                  << "rec.t = " <<  rec.t << '\n'
                  << "rec.p = " <<  rec.p << '\n';
    }

    rec.normal = Vec3(1, 0, 0);
    rec.front_face = true;
    rec.mat = phase_function;

    return true;
}

bool ConstantMedium::bounding_box(double time0, double time1, Aabb &output_box) const
{
    return boundary->bounding_box(time0, time1, output_box);
}
