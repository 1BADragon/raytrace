#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <memory>
#include <unordered_map>

#include <scene.h>
#include <textures/texture.h>
#include <materials/material.h>

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

#endif // SCENEBUILDER_H
