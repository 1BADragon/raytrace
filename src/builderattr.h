#ifndef BUILDERATTR_H
#define BUILDERATTR_H

#include <stdexcept>
#include <memory>
#include <vector>
#include <unordered_map>
#include <variant>

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
    using ItemVec = std::vector<std::shared_ptr<BuilderAttr>>;
    using ItemMap = std::unordered_map<std::string, std::shared_ptr<BuilderAttr>>;
    using Item = std::variant<double, std::string, ItemVec, ItemMap>;

    static std::shared_ptr<BuilderAttr> number(double d);
    static std::shared_ptr<BuilderAttr> number(int d);
    static std::shared_ptr<BuilderAttr> string(const std::string &s);
    static std::shared_ptr<BuilderAttr> object();
    static std::shared_ptr<BuilderAttr> array();

    enum ItemType {
        NUMBER,
        STRING,
        ARRAY,
        OBJECT
    };

    ItemType type() const {
        return static_cast<ItemType>(item.index());
    }

    int as_int() const;
    double as_double() const;
    std::string as_string() const;

    std::shared_ptr<BuilderAttr>& operator[](size_t index);
    std::shared_ptr<BuilderAttr>& operator[](const std::string &key);

    void push_new(std::shared_ptr<BuilderAttr> ba);

    size_t n_children() const;
    bool has_child(size_t index) const;
    bool has_child(const std::string &key) const;

    std::shared_ptr<BuilderAttr> child(size_t index) const;
    std::shared_ptr<BuilderAttr> child(const std::string &key) const;

private:
    BuilderAttr(int i) : item(static_cast<double>(i)) {}
    BuilderAttr(double f) : item(f) {}
    BuilderAttr(const std::string &s) : item(s) {}

    Item item;
};

#endif // BUILDERATTR_H
