#include <builderattr.h>

#include <color.h>

std::shared_ptr<BuilderAttr> BuilderAttr::number(double d)
{
    auto b = std::shared_ptr<BuilderAttr>(new BuilderAttr(d));
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::number(int d)
{
    auto b = std::shared_ptr<BuilderAttr>(new BuilderAttr(d));
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::string(const std::string &s)
{
    auto b = std::shared_ptr<BuilderAttr>(new BuilderAttr(s));
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::object()
{
    auto b = std::shared_ptr<BuilderAttr>(new BuilderAttr(0));
    b->item = ItemMap();
    return b;
}

std::shared_ptr<BuilderAttr> BuilderAttr::array()
{
    auto b = std::shared_ptr<BuilderAttr>(new BuilderAttr(0));
    b->item = ItemVec();
    return b;
}

int BuilderAttr::as_int() const
{
    if (type() != NUMBER) {
        throw BuilderAttrError("Error reading value: not a number");
    }

    return static_cast<int>(std::get<double>(item));
}

double BuilderAttr::as_double() const
{
    if (type() != NUMBER) {
        throw BuilderAttrError("Error reading value: not a number");
    }

    return std::get<double>(item);
}

std::string BuilderAttr::as_string() const
{
    if (type() != STRING) {
        throw BuilderAttrError("Error reading value: not a string");
    }

    return std::get<std::string>(item);
}

std::shared_ptr<BuilderAttr> &BuilderAttr::operator [](size_t index)
{
    if (type() != ARRAY) {
        throw BuilderAttrError("Error reading value: not an array");
    }

    return std::get<ItemVec>(item)[index];
}

void BuilderAttr::push_new(std::shared_ptr<BuilderAttr> ba)
{
    if (type() != ARRAY) {
        throw BuilderAttrError("Pushing to a non array type");
    }

    std::get<ItemVec>(item).push_back(ba);
}

bool BuilderAttr::has_child(size_t index) const
{
    if (type() != ARRAY) {
        return false;
    }

    return std::get<ItemVec>(item).size() > index;
}

bool BuilderAttr::has_child(const std::string &key) const
{
    if (type() != OBJECT) {
        return false;
    }

    return std::get<ItemMap>(item).count(key) > 0;
}

std::shared_ptr<BuilderAttr> BuilderAttr::child(size_t index) const
{
    if (type() != ARRAY) {
        throw BuilderAttrError("Attr not an array");
    }

    return std::get<ItemVec>(item).at(index);
}

std::shared_ptr<BuilderAttr> BuilderAttr::child(const std::string &key) const
{
    if (type() != OBJECT) {
        throw BuilderAttrError("Error reading value: not an object");
    }

    return std::get<ItemMap>(item).at(key);
}

std::shared_ptr<BuilderAttr>& BuilderAttr::operator[](const std::string &key)
{
    if (type() != OBJECT) {
        throw BuilderAttrError("Error reading value: not an object");
    }

    return std::get<ItemMap>(item)[key];
}
