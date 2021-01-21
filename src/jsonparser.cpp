#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cjson/cJSON.h>
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

}
