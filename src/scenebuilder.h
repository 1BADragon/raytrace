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

    static std::shared_ptr<Texture> build_texture(const std::shared_ptr<BuilderAttr> ba);
    static std::shared_ptr<Material> build_material(const std::shared_ptr<BuilderAttr> ba);
    static std::shared_ptr<Hittable> build_hittable(const std::shared_ptr<BuilderAttr> ba);

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
};

class BuilderAttr
{
    class BuilderAttrError : public std::exception
    {
    public:
        BuilderAttrError(const std::string &w) : _what(w) {}

        const char * what() const throw() override {
            return _what.c_str();
        }
    private:
        std::string _what;
    };

public:
    using ItemVec = std::vector<std::shared_ptr<BuilderAttr>>;
    using ItemMap = std::unordered_map<std::string, std::shared_ptr<BuilderAttr>>;
    using Item = std::variant<double, std::string, ItemVec, ItemMap>;

    static std::shared_ptr<BuilderAttr> number(double d);
    static std::shared_ptr<BuilderAttr> number(int d);
    static std::shared_ptr<BuilderAttr> string(const std::string &s);
    static std::shared_ptr<BuilderAttr> object();
    static std::shared_ptr<BuilderAttr> array();

    enum ItemType {
        NUMBER,
        STRING,
        ARRAY,
        OBJECT
    };

    ItemType type() const {
        return static_cast<ItemType>(item.index());
    }

    int as_int() const;
    double as_double() const;
    std::string as_string() const;

    std::shared_ptr<BuilderAttr>& operator[](size_t index);
    std::shared_ptr<BuilderAttr>& operator[](const std::string &key);

    void push_new(std::shared_ptr<BuilderAttr> ba);

    size_t n_children() const;
    bool has_child(size_t index) const;
    bool has_child(const std::string &key) const;

    std::shared_ptr<BuilderAttr> child(size_t index) const;
    std::shared_ptr<BuilderAttr> child(const std::string &key) const;

private:
    BuilderAttr(int i) : item(static_cast<double>(i)) {}
    BuilderAttr(double f) : item(f) {}
    BuilderAttr(const std::string &s) : item(s) {}

    Item item;
};

#endif // SCENEBUILDER_H
