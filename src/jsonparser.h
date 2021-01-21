#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <memory>

#include <scene.h>

class JsonParser
{
public:
    JsonParser();

    std::shared_ptr<Scene> build_from_file(const std::string &filename) const;
    std::shared_ptr<Scene> build_from_string(const std::string &content) const;
};

#endif // JSONPARSER_H
