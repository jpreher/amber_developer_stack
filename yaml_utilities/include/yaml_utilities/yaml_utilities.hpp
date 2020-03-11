/**
 * @brief Command line interface for HZD ROS node
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef _COMMON_YAML_UTILITIES_HPP_
#define _COMMON_YAML_UTILITIES_HPP_

#include <ostream>
#include <yaml-cpp-0.2.7/yaml.h>
#include <boost/filesystem.hpp>

#ifndef YAML_UTITILIES_NO_ALIAS
#define YAML YAML_0_2_7
#endif

namespace yaml_utilities
{

void yaml_read_file(const boost::filesystem::path &filePath, YAML::Node &doc);
void yaml_write_file(const boost::filesystem::path &filePath, const YAML::Emitter &out);

void yaml_read_string(const std::string &str, YAML::Node &doc);
void yaml_write_string(std::string &str, YAML::Emitter const &out);


/**
 * \brief Read one line from std::cin.
 */
void yaml_read_stdin(YAML::Node &doc, bool ignoreFirst = true);

template<typename T>
void yaml_read_string(const std::string &str, T &var)
{
    YAML::Node doc;
    yaml_read_string(str, doc);
    doc >> var;
}

template<typename K, typename T>
void yaml_read_default(const YAML::Node &doc, const K &key, T &value, const T &def = T())
{
    const YAML::Node *pNode = doc.FindValue(key);
    if (pNode)
        (*pNode) >> value;
    else
        value = def;
}

inline void yaml_read_default(const YAML::Node &doc, const std::string &key, std::string &value, const std::string &def = std::string())
{
    yaml_read_default<std::string, std::string>(doc, key, value, def);
}

template<typename K, typename T>
T yaml_get_default(const YAML::Node &doc, const K &key, const T &def = T())
{
    T value;
    yaml_read_default(doc, key, value, def);
    return value;
}

/**
 * \brief Try reading value from a file specified as a string (relative to given path), otherwise read the value directly from the entry
 * \note Not good for sent files
 */
template<typename T>
void yaml_read_or_load(const YAML::Node &node, T &value, boost::filesystem::path relPath = "", const std::string &loadName = "")
{
    std::string nameTest;
    if (node.Read(nameTest))
    {
        // It's a file
        YAML::Node newNode;
        yaml_read_file(relPath / (nameTest + ".yml"), newNode);
        if (loadName.empty())
            newNode >> value;
        else
            newNode[loadName] >> value;
    }
    else
        node >> value;
}


class YamlDumpable
{
public:
    inline virtual ~YamlDumpable() { }
    virtual void dump(YAML::Emitter &out) const = 0;
};

template<typename T>
class YamlDumpableT
{
public:
    friend YAML::Emitter &operator<<(YAML::Emitter &out, const T &obj)
    {
        obj.dump(out);
        return out;
    }
};

class YamlLoadable
{
public:
    inline virtual ~YamlLoadable() { }
    virtual void load(const YAML::Node &in) = 0;
};

// Just do pure templates?
template<typename T>
class YamlLoadableT : public YamlLoadable
{
public:
    friend void operator>>(const YAML::Node &in, T &obj)
    {
        obj.load(in);
    }
};

} // namespace common


namespace YAML
{

typedef boost::shared_ptr<Node> NodePtr;
typedef boost::shared_ptr<const Node> NodeConstPtr;

Emitter &operator<<(Emitter &out, const boost::filesystem::path &path);

void operator>>(const Node &in, boost::filesystem::path &path);
Emitter &operator<<(Emitter &out, const boost::filesystem::path &path);

Emitter &operator<<(Emitter &out, const yaml_utilities::YamlDumpable &obj);
void operator>>(const Node &in, yaml_utilities::YamlLoadable &obj);

} // namespace YAML

std::ostream &operator<<(std::ostream &os, const YAML::Emitter &out);
std::ostream &operator<<(std::ostream &os, const yaml_utilities::YamlDumpable &obj);

#endif
