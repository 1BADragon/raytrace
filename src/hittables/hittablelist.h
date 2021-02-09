#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <vector>
#include <memory>
#include <hittables/hittable.h>

class HittableList : public Hittable
{
public:
    HittableList() : _objects() {}

    void clear() { _objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { _objects.push_back(object); }

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

    virtual bool bounding_box(double time0, double time1, Aabb &output_box) const override;

    virtual double pdf_value(const Vec3 &o, const Vec3 &v) const override;

    virtual Vec3 random(const Vec3 &o) const override;

    const std::vector<std::shared_ptr<Hittable>>& objects() const;

    size_t n_objects() const { return _objects.size(); }

private:
    std::vector<std::shared_ptr<Hittable>> _objects;
};

#endif // HITTABLELIST_H
