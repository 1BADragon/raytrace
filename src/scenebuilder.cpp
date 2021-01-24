#include <stdexcept>
#include <sstream>

#include <scenebuilder.h>
#include <builderattr.h>
#include <hittables.h>
#include <textures.h>
#include <materials.h>

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

static Color build_color(std::shared_ptr<BuilderAttr> c);
static Point3 build_point(std::shared_ptr<BuilderAttr> p);
static Vec3 build_vec(std::shared_ptr<BuilderAttr> v);

SceneBuilder::SceneBuilder() :
    _scene(std::make_shared<Scene>()), _textures(), _materials()
{

}

std::shared_ptr<Texture> SceneBuilder::build_texture(const std::shared_ptr<BuilderAttr> ba)
{
    if (!ba->has_child("type")) {
        throw SceneBuildingError("Textures required a \"type\" field");
    }
    const std::string type = ba->child("type")->as_string();

    if (type == "solid_color") {
        return build_solid_color(ba);
    } else if (type == "checker") {
        return build_checker(ba);
    } else if (type == "noise") {
        return build_noise(ba);
    } else if (type == "image") {
        return build_image(ba);
    } else {
        throw SceneBuildingError("Unsupported texture type: \"" + type + "\"");
    }
}

std::shared_ptr<Material> SceneBuilder::build_material(const std::shared_ptr<BuilderAttr> ba)
{
    if (!ba->has_child("type")) {
        throw SceneBuildingError("Materials require a \"type\" field");
    }
    const std::string type = ba->child("type")->as_string();

    if (type == "metal") {
        return build_metal(ba);
    } else if (type == "lambertian") {
        return build_lambertain(ba);
    } else if (type == "dielectric") {
        return build_dielectric(ba);
    } else if (type == "diffuselight") {
        return build_diffuselight(ba);
    } else {
        throw SceneBuildingError("Unsupported material type: \"" + type + "\"");
    }
}

std::shared_ptr<Hittable> SceneBuilder::build_hittable(const std::shared_ptr<BuilderAttr> ba)
{
    if (!ba->has_child("type")) {
        throw SceneBuildingError("Objects require a \"type\" field");
    }
    const std::string type = ba->child("type")->as_string();

    if (type == "sphere") {
        return build_sphere(ba);
    } else {
        throw SceneBuildingError("Unknown object type: \"" + type + '"');
    }
}

void SceneBuilder::set_aspect_ratio(const std::shared_ptr<BuilderAttr> v)
{
    if (v->type() != BuilderAttr::NUMBER) {
        throw SceneBuildingError("Aspect ratio value needs to be a number");
    }

    this->scene()->set_aspect_ratio(v->as_double());
}

void SceneBuilder::set_image_width(const std::shared_ptr<BuilderAttr> v)
{
    if (v->type() != BuilderAttr::NUMBER) {
        throw SceneBuildingError("Image width value needs to be a number");
    }

    this->scene()->set_image_width(v->as_int());
}

void SceneBuilder::set_samples_per_pixel(const std::shared_ptr<BuilderAttr> v)
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

    this->scene()->set_max_depth(v->as_int());
}

void SceneBuilder::set_background(const std::shared_ptr<BuilderAttr> v)
{
    this->scene()->set_background(build_color(v));
}

void SceneBuilder::set_camera_attrs(const std::shared_ptr<BuilderAttr> v)
{
    Point3 lookfrom = build_point(v->child("lookfrom"));
    Point3 lookat = build_point(v->child("lookat"));
    Vec3 vup = build_vec(v->child("vup"));
    double vfov = v->child("vfov")->as_double();
    double aperture = v->child("aperture")->as_double();
    double focus_dist = v->child("focus_dist")->as_double();
    double time0 = v->child("time0")->as_double();
    double time1 = v->child("time1")->as_double();

    this->scene()->set_camera(std::make_shared<Camera>(lookfrom, lookat, vup, vfov,
                                                       this->scene()->aspect_ratio(),
                                                       aperture, focus_dist, time0,
                                                       time1));
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

void SceneBuilder::add_object_to_scene(std::shared_ptr<Hittable> h)
{
    scene()->add_object(h);
}

std::shared_ptr<Texture> SceneBuilder::parse_texture(std::shared_ptr<BuilderAttr> ba)
{
    switch (ba->type()) {
    case BuilderAttr::STRING:
        return this->texture(ba->as_string());
    case BuilderAttr::ARRAY:
        return std::make_shared<SolidColor>(build_color(ba));
    case BuilderAttr::OBJECT:
        return build_texture(ba);
    default:
        throw SceneBuildingError("Invalid data for texture");
    }
}

std::shared_ptr<Material> SceneBuilder::parse_material(std::shared_ptr<BuilderAttr> ba)
{
    switch (ba->type()) {
    case BuilderAttr::STRING:
        return this->material(ba->as_string());
    case BuilderAttr::OBJECT:
        return build_material(ba);
    default:
        throw SceneBuildingError("Invalid material format");
    }
}

std::shared_ptr<Texture> SceneBuilder::build_solid_color(std::shared_ptr<BuilderAttr> ba)
{
    Color c = build_color(ba->child("value"));
    return std::make_shared<SolidColor>(c.x(), c.y(), c.z());
}

std::shared_ptr<Texture> SceneBuilder::build_checker(std::shared_ptr<BuilderAttr> ba)
{
    auto color1 = parse_texture(ba->child("color1"));
    auto color2 = parse_texture(ba->child("color2"));

    return std::make_shared<CheckerTexture>(color1, color2);
}

std::shared_ptr<Texture> SceneBuilder::build_noise(std::shared_ptr<BuilderAttr> ba)
{
    double scale = ba->child("scale")->as_double();

    return std::make_shared<NoiseTexture>(scale);
}

std::shared_ptr<Texture> SceneBuilder::build_image(std::shared_ptr<BuilderAttr> ba)
{
    std::string path = ba->child("path")->as_string();

    return std::make_shared<ImageTexture>(path);
}

std::shared_ptr<Material> SceneBuilder::build_metal(std::shared_ptr<BuilderAttr> ba)
{
    Color c = build_color(ba->child("color"));
    double f = ba->child("fuzz")->as_double();

    return std::make_shared<Metal>(c, f);
}

std::shared_ptr<Material> SceneBuilder::build_lambertain(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<Lambertian>(parse_texture(ba->child("texture")));
}

std::shared_ptr<Material> SceneBuilder::build_dielectric(std::shared_ptr<BuilderAttr> ba)
{
    double i = ba->child("index")->as_double();

    return std::make_shared<Dielectric>(i);
}

std::shared_ptr<Material> SceneBuilder::build_diffuselight(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<DiffuseLight>(parse_texture(ba->child("texture")));
}

std::shared_ptr<Hittable> SceneBuilder::build_sphere(std::shared_ptr<BuilderAttr> ba)
{
    if (ba->has_child("time0")) {
        Point3 cen0 = build_point(ba->child("center0"));
        Point3 cen1 = build_point(ba->child("center1"));
        double time0 = ba->child("time0")->as_double();
        double time1 = ba->child("time1")->as_double();
        double r = ba->child("radius")->as_double();
        auto m = parse_material(ba->child("material"));

        return std::make_shared<Sphere>(cen0, cen1, time0, time1, r, m);
    } else {
        Point3 center = build_point(ba->child("center"));
        double r = ba->child("radius")->as_double();
        auto m = parse_material(ba->child("material"));

        return std::make_shared<Sphere>(center, r, m);
    }
}

static Color build_color(std::shared_ptr<BuilderAttr> c)
{
    double r = (*c)[0]->as_double();
    double g = (*c)[1]->as_double();
    double b = (*c)[2]->as_double();

    return Color(r, g, b);
}

static Point3 build_point(std::shared_ptr<BuilderAttr> p)
{
    double x = (*p)[0]->as_double();
    double y = (*p)[1]->as_double();
    double z = (*p)[2]->as_double();

    return Point3(x, y, z);
}

static Vec3 build_vec(std::shared_ptr<BuilderAttr> v)
{
    double i = (*v)[0]->as_double();
    double j = (*v)[1]->as_double();
    double k = (*v)[2]->as_double();

    return Vec3(i, j, k);
}
