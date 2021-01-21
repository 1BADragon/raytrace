#include <stdexcept>
#include <sstream>

#include <scenebuilder.h>

class SceneBuildingError : public std::exception
{
public:
    SceneBuildingError(const std::string &what) : _what(what)
    {

    }

    const char * what() const throw() override
    {
        return _what.c_str();
    }

private:
    std::string _what;
};


SceneBuilder::SceneBuilder() :
    _scene(std::make_shared<Scene>()), _textures(), _materials()
{

}

std::shared_ptr<Scene> SceneBuilder::scene() const
{
    return _scene;
}

bool SceneBuilder::have_texture(const std::string &name) const
{
    return _textures.count(name) != 0;
}

bool SceneBuilder::have_material(const std::string &name) const
{
    return _materials.count(name) != 0;
}

void SceneBuilder::insert_texture(const std::string &name, std::shared_ptr<Texture> t)
{
    if (have_texture(name)) {
        std::stringstream ss;
        ss << "Error building texture '" << name << "', texture with same name already exists.";
        throw SceneBuildingError(ss.str());
    }

    _textures.insert(std::make_pair(name, t));
}

void SceneBuilder::insert_material(const std::string &name, std::shared_ptr<Material> m)
{
    if (have_material(name)) {
        std::stringstream ss;
        ss << "Error building material '" << name << "', material with same name already exists.";
        throw SceneBuildingError(ss.str());
    }

    _materials.insert(std::make_pair(name, m));
}

std::shared_ptr<Texture> SceneBuilder::texture(const std::string &name)
{
    if (!have_texture(name)) {
        std::stringstream ss;
        ss << "Unable to access texture, '" << name << "'";
        throw SceneBuildingError(ss.str());
    }

    return _textures.at(name);
}

std::shared_ptr<Material> SceneBuilder::material(const std::string &name)
{
    if (!have_material(name)) {
        std::stringstream ss;
        ss << "Unable to access material, '" << name << "'";
        throw SceneBuildingError(ss.str());
    }

    return _materials.at(name);
}
