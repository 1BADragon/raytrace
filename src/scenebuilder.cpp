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
    insert_material("none", std::make_shared<Material>());
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
    } else if (type == "box") {
        return build_box(ba);
    } else if (type == "constant_medium") {
        return build_constant_medium(ba);
    } else if (type == "xy_rect") {
        return build_xy_rect(ba);
    } else if (type == "xz_rect") {
        return build_xz_rect(ba);
    } else if (type == "yz_rect") {
        return build_yz_rect(ba);
    } else if (type == "rotate") {
        return build_rotatey(ba);
    } else if (type == "translate") {
        return build_translate(ba);
    } else if (type == "flipface") {
        return build_flipface(ba);
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

void SceneBuilder::add_light_to_scene(std::shared_ptr<Hittable> l)
{
    scene()->add_light(l);
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
    Point3 center = build_point(ba->child("center"));
    double r = ba->child("radius")->as_double();
    auto m = parse_material(ba->child("material"));

    return std::make_shared<Sphere>(center, r, m);
}

std::shared_ptr<Hittable> SceneBuilder::build_box(std::shared_ptr<BuilderAttr> ba)
{
    Point3 p0 = build_point(ba->child("point0"));
    Point3 p1 = build_point(ba->child("point1"));
    auto mat = parse_material(ba->child("material"));

    return std::make_shared<Box>(p0, p1, mat);
}

std::shared_ptr<Hittable> SceneBuilder::build_constant_medium(std::shared_ptr<BuilderAttr> ba)
{
    auto obj = build_hittable(ba->child("object"));
    double d = ba->child("density")->as_double();
    auto mat = parse_texture(ba->child("texture"));

    return std::make_shared<ConstantMedium>(obj, d, mat);
}

std::shared_ptr<Hittable> SceneBuilder::build_xy_rect(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<XYRect>(
                ba->child("x0")->as_double(),
                ba->child("x1")->as_double(),
                ba->child("y0")->as_double(),
                ba->child("y1")->as_double(),
                ba->child("k")->as_double(),
                parse_material(ba->child("material"))
                );
}

std::shared_ptr<Hittable> SceneBuilder::build_xz_rect(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<XZRect>(
                ba->child("x0")->as_double(),
                ba->child("x1")->as_double(),
                ba->child("z0")->as_double(),
                ba->child("z1")->as_double(),
                ba->child("k")->as_double(),
                parse_material(ba->child("material"))
                );
}

std::shared_ptr<Hittable> SceneBuilder::build_yz_rect(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<YZRect>(
                ba->child("y0")->as_double(),
                ba->child("y1")->as_double(),
                ba->child("z0")->as_double(),
                ba->child("z1")->as_double(),
                ba->child("k")->as_double(),
                parse_material(ba->child("material"))
                );
}

std::shared_ptr<Hittable> SceneBuilder::build_rotatey(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<Rotate>(
                build_hittable(ba->child("object")),
                ba->child("x")->as_double(),
                ba->child("y")->as_double(),
                ba->child("z")->as_double()
                );
}

std::shared_ptr<Hittable> SceneBuilder::build_translate(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<Translate>(
                build_hittable(ba->child("object")),
                build_vec(ba->child("vec"))
                );
}

std::shared_ptr<Hittable> SceneBuilder::build_flipface(std::shared_ptr<BuilderAttr> ba)
{
    return std::make_shared<FlipFace>(build_hittable(ba->child("object")));
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
