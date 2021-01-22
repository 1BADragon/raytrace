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

std::shared_ptr<Texture> SceneBuilder::build_texture(std::shared_ptr<BuilderAttr> ba)
{
    return nullptr;
}

std::shared_ptr<Material> SceneBuilder::build_material(std::shared_ptr<BuilderAttr> ba)
{
    return nullptr;
}

std::shared_ptr<Hittable> SceneBuilder::build_hittable(std::shared_ptr<BuilderAttr> ba)
{
    return nullptr;
}

void SceneBuilder::set_aspect_ratio(std::shared_ptr<BuilderAttr> v)
{
    if (v->type() != BuilderAttr::NUMBER) {
        throw SceneBuildingError("Aspect ratio value needs to be a number");
    }

    this->scene()->set_aspect_ratio(v->as_double());
}

void SceneBuilder::set_image_width(std::shared_ptr<BuilderAttr> v)
{
    if (v->type() != BuilderAttr::NUMBER) {
        throw SceneBuildingError("Image width value needs to be a number");
    }

    this->scene()->set_image_width(v->as_int());
}

void SceneBuilder::set_samples_per_pixel(std::shared_ptr<BuilderAttr> v)
{
    if (v->type() != BuilderAttr::NUMBER) {
        throw SceneBuildingError("Samples per pixel needs to be a number");
    }

    this->scene()->set_samples_per_pixel(v->as_int());
}

void SceneBuilder::set_max_depth(std::shared_ptr<BuilderAttr> v)
{
    if (v->type() != BuilderAttr::NUMBER) {
        throw SceneBuildingError("Max depth needs to be a number");
    }

    this->scene()->set_samples_per_pixel(v->as_int());
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

std::shared_ptr<BuilderAttr> BuilderAttr::number(double d)
{
    auto b = std::make_shared<BuilderAttr>(d);
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::number(int d)
{
    auto b = std::make_shared<BuilderAttr>(d);
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::string(const std::string &s)
{
    auto b = std::make_shared<BuilderAttr>(s);
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::object()
{
    auto b = std::make_shared<BuilderAttr>();
    b->item = ItemMap();
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::array()
{
    auto b = std::make_shared<BuilderAttr>();
    b->item = ItemVec();
    return b;
}

int BuilderAttr::as_int() const
{
    if (type() != NUMBER) {
        throw SceneBuildingError("Error reading value: not a number");
    }

    return static_cast<int>(std::get<double>(item));
}

double BuilderAttr::as_double() const
{
    if (type() != NUMBER) {
        throw SceneBuildingError("Error reading value: not a number");
    }

    return std::get<double>(item);
}

std::string BuilderAttr::as_string() const
{
    if (type() != STRING) {
        throw SceneBuildingError("Error reading value: not a string");
    }

    return std::get<std::string>(item);
}

std::shared_ptr<BuilderAttr> &BuilderAttr::operator [](size_t index)
{
    if (type() != ARRAY) {
        throw SceneBuildingError("Error reading value: not an array");
    }

    return std::get<ItemVec>(item)[index];
}

void BuilderAttr::push_new(std::shared_ptr<BuilderAttr> ba)
{
    if (type() != ARRAY) {
        throw SceneBuildingError("Pushing to a non array type");
    }

    std::get<ItemVec>(item).push_back(ba);
}

std::shared_ptr<BuilderAttr>& BuilderAttr::operator[](const std::string &key)
{
    if (type() != OBJECT) {
        throw SceneBuildingError("Error reading value: not an object");
    }

    return std::get<ItemMap>(item)[key];
}
