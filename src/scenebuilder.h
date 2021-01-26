#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <memory>
#include <variant>
#include <stdexcept>
#include <unordered_map>

#include <scene.h>
#include <textures/texture.h>
#include <materials/material.h>
#include <hittables/hittable.h>

class BuilderAttr;

class SceneBuilder
{
public:
    SceneBuilder();

    std::shared_ptr<Texture> build_texture(const std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Material> build_material(const std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_hittable(const std::shared_ptr<BuilderAttr> ba);

    void set_aspect_ratio(const std::shared_ptr<BuilderAttr> v);
    void set_image_width(const std::shared_ptr<BuilderAttr> v);
    void set_samples_per_pixel(const std::shared_ptr<BuilderAttr> v);
    void set_max_depth(const std::shared_ptr<BuilderAttr> v);
    void set_background(const std::shared_ptr<BuilderAttr> v);
    void set_camera_attrs(const std::shared_ptr<BuilderAttr> v);

    std::shared_ptr<Scene> scene() const;
    bool have_texture (const std::string &name) const;
    bool have_material (const std::string &name) const;

    void insert_texture (const std::string &name, std::shared_ptr<Texture> t);
    void insert_material (const std::string &name, std::shared_ptr<Material> m);

    std::shared_ptr<Texture> texture(const std::string &name);
    std::shared_ptr<Material> material(const std::string &name);

    void add_object_to_scene(std::shared_ptr<Hittable> h);

private:
    std::shared_ptr<Scene> _scene;

    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
    std::unordered_map<std::string, std::shared_ptr<Material>> _materials;

    std::shared_ptr<Texture> parse_texture(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Material> parse_material(std::shared_ptr<BuilderAttr> ba);

    std::shared_ptr<Texture> build_solid_color(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Texture> build_checker(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Texture> build_noise(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Texture> build_image(std::shared_ptr<BuilderAttr> ba);

    std::shared_ptr<Material> build_metal(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Material> build_lambertain(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Material> build_dielectric(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Material> build_diffuselight(std::shared_ptr<BuilderAttr> ba);

    std::shared_ptr<Hittable> build_sphere(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_box(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_constant_medium(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_xy_rect(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_xz_rect(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_yz_rect(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_rotatey(std::shared_ptr<BuilderAttr> ba);
    std::shared_ptr<Hittable> build_translate(std::shared_ptr<BuilderAttr> ba);
};



#endif // SCENEBUILDER_H
