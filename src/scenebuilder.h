#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <memory>
#include <variant>
#include <stdexcept>
#include <unordered_map>

#include <scene.h>
#include <textures/texture.h>
#include <materials/material.h>

class BuilderAttr;

class SceneBuilder
{
public:
    SceneBuilder();

    std::shared_ptr<Scene> scene() const;
    bool have_texture (const std::string &name) const;
    bool have_material (const std::string &name) const;

    void insert_texture (const std::string &name, std::shared_ptr<Texture> t);
    void insert_material (const std::string &name, std::shared_ptr<Material> m);

    std::shared_ptr<Texture> texture(const std::string &name);
    std::shared_ptr<Material> material(const std::string &name);

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
    using Item = std::variant<float, std::string,
        std::vector<std::shared_ptr<BuilderAttr>>,
        std::unordered_map<std::string, std::shared_ptr<BuilderAttr>>>;

    enum ItemType {
        NUMBER,
        STRING,
        ARRAY,
        OBJECT
    };

    ItemType get_type() const {
        return static_cast<ItemType>(item.index());
    }

    int as_int() const;
    float as_float() const;
    std::string as_string() const;

    std::shared_ptr<BuilderAttr> operator[](size_t index) const;
    std::shared_ptr<BuilderAttr> operator[](const std::string &key) const;

private:
    Item item;
};

#endif // SCENEBUILDER_H
