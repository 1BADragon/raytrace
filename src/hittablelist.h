#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include <vector>
#include <memory>
#include <hittable.h>

class HittableList : public Hittable
{
public:
    HittableList() : objects() {}

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray &r, double min, double max, HitRecord &rec) const override;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

#endif // HITTABLELIST_H
