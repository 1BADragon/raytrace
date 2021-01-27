#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cjson/cJSON.h>
#include <builderattr.h>
#include <scenebuilder.h>
#include <jsonparser.h>

class JsonParsingError : public std::exception
{
public:
    JsonParsingError(const std::string & w) : _what(w) {}

    const char * what() const throw() override {
        return _what.c_str();
    }

private:
    std::string _what;
};

using sbptr = std::shared_ptr<SceneBuilder>;

static void begin_parsing(sbptr sb, cJSON *j);
static void parse_scene_attrs(sbptr sb, cJSON *j);
static void parse_textures(sbptr sb, cJSON *j);
static std::shared_ptr<Texture> parse_texture(sbptr sb, cJSON *j);
static void parse_materials(sbptr sb, cJSON *j);
static std::shared_ptr<Material> parse_material(sbptr sb, cJSON *j);
static void parse_objects(sbptr sb, cJSON *j);
static std::shared_ptr<Hittable> parse_object(sbptr sb, cJSON *j);
static void parse_lights(sbptr sb, cJSON *j);

static std::shared_ptr<BuilderAttr> build_attr(cJSON *j);

JsonParser::JsonParser()
{

}

std::shared_ptr<Scene> JsonParser::build_from_file(const std::string &filename) const
{
    std::ifstream fin(filename);
    std::string content;

    if (!fin) {
        std::stringstream ss;
        ss << "Unable to open '" << filename << "'";
        throw JsonParsingError(ss.str());
    }

    content = std::string((std::istreambuf_iterator<char>(fin)),
                          std::istreambuf_iterator<char>());
    return build_from_string(content);
}

std::shared_ptr<Scene> JsonParser::build_from_string(const std::string &content) const
{
    struct cjson_deleter {
        void operator()(cJSON *j) {
            cJSON_free(j);
        }
    };

    auto builder = std::make_shared<SceneBuilder>();
    std::unique_ptr<cJSON, cjson_deleter> j(cJSON_Parse(content.c_str()));

    if (!j) {
        std::stringstream ss;
        ss << "Unable to parse json content";
        throw JsonParsingError(ss.str());
    }

    begin_parsing(builder, j.get());

    return builder->scene();
}

static void begin_parsing(sbptr sb, cJSON *j)
{
    if (!cJSON_IsObject(j)) {
        throw JsonParsingError("Top level entity must be an object");
    }

    // First locate and parse the attributes of the scene to be drawn
    parse_scene_attrs(sb, j);
    parse_textures(sb, j);
    parse_materials(sb, j);
    parse_objects(sb, j);
    parse_lights(sb, j);
}

static void parse_scene_attrs(sbptr sb, cJSON *j)
{
    cJSON *tmp;

    tmp = cJSON_GetObjectItem(j, "aspect_ratio");
    sb->set_aspect_ratio(build_attr(tmp));

    tmp = cJSON_GetObjectItem(j, "image_width");
    sb->set_image_width(build_attr(tmp));

    tmp = cJSON_GetObjectItem(j, "samples_per_pixel");
    sb->set_samples_per_pixel(build_attr(tmp));

    tmp = cJSON_GetObjectItem(j, "max_depth");
    sb->set_max_depth(build_attr(tmp));

    tmp = cJSON_GetObjectItem(j, "background");
    sb->set_background(build_attr(tmp));

    tmp = cJSON_GetObjectItem(j, "camera");
    sb->set_camera_attrs(build_attr(tmp));
}

static std::shared_ptr<BuilderAttr> build_attr(cJSON *j)
{
    if (j == nullptr) {
        throw JsonParsingError("NULL pointer encountered");
    }

    if (cJSON_IsNumber(j)) {
        return BuilderAttr::number(cJSON_GetNumberValue(j));
    } else if (cJSON_IsString(j)) {
        return BuilderAttr::string(cJSON_GetStringValue(j));
    } else if (cJSON_IsArray(j)) {
        cJSON *chld;
        auto arr = BuilderAttr::array();
        cJSON_ArrayForEach(chld, j) {
            arr->push_new(build_attr(chld));
        }
        return arr;
    } else if (cJSON_IsObject(j)) {
        cJSON *chld;
        auto obj = BuilderAttr::object();
        cJSON_ArrayForEach(chld, j) {
            std::string key(chld->string);
            (*obj)[key] = build_attr(cJSON_GetObjectItem(j, key.c_str()));
        }
        return obj;
    } else {
        throw JsonParsingError("Unsupported type encountered");
    }
}

static void parse_textures(sbptr sb, cJSON *j)
{
    cJSON *chld = nullptr;
    cJSON *textures = cJSON_GetObjectItem(j, "textures");

    cJSON_ArrayForEach(chld, textures) {
        std::string key = chld->string;

        auto texture = parse_texture(sb, cJSON_GetObjectItem(textures, key.c_str()));
        sb->insert_texture(key, texture);
    }
}

static std::shared_ptr<Texture> parse_texture(sbptr sb, cJSON *j)
{
    return sb->build_texture(build_attr(j));
}

static void parse_materials(sbptr sb, cJSON *j)
{
    cJSON *chld = nullptr;
    cJSON *materials = cJSON_GetObjectItem(j, "materials");

    cJSON_ArrayForEach(chld, materials) {
        std::string key = chld->string;

        auto material = parse_material(sb, cJSON_GetObjectItem(materials, key.c_str()));
        sb->insert_material(key, material);
    }
}

static std::shared_ptr<Material> parse_material(sbptr sb, cJSON *j)
{
    return sb->build_material(build_attr(j));
}

static void parse_objects(sbptr sb, cJSON *j)
{
    cJSON *chld = nullptr;
    cJSON *objects = cJSON_GetObjectItem(j, "objects");

    cJSON_ArrayForEach(chld, objects) {
        auto obj = parse_object(sb, chld);

        sb->add_object_to_scene(obj);
    }
}

static std::shared_ptr<Hittable> parse_object(sbptr sb, cJSON *j)
{
    return sb->build_hittable(build_attr(j));
}

static void parse_lights(sbptr sb, cJSON *j)
{
    cJSON *chld = nullptr;
    cJSON *lights = cJSON_GetObjectItem(j, "lights");

    cJSON_ArrayForEach(chld, lights) {
        auto light = parse_object(sb, chld);

        sb->add_light_to_scene(light);
    }
}
