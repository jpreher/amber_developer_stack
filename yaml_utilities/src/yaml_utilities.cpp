/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */

#ifndef _YAML_UTILITIES_YAML_UTILITIES_H
    #define _YAML_UTILITIES_YAML_UTILITIES_H

#include <fstream>
#include <sstream>

#include <yaml_utilities/yaml_utilities.hpp>

using namespace YAML;
using namespace std;

namespace yaml_utilities
{

void yaml_read_stream(std::istream &stream, YAML::Node &doc)
{
    YAML::Parser parser(stream);
    parser.GetNextDocument(doc);
}

void yaml_read_file(boost::filesystem::path const &filePath, YAML::Node &doc)
{
    if (!boost::filesystem::exists(filePath))
    {
        ostringstream os;
        os << "yaml_read_file: file '" << filePath << "' does not exist";
        cout << os.str() << endl;
        throw std::runtime_error(os.str());
    }

    std::ifstream fin(filePath.string().c_str());
    yaml_read_stream(fin, doc);
    fin.close();
}

void yaml_read_string(std::string const &str, YAML::Node &doc)
{
    std::istringstream sstr(str);
    yaml_read_stream(sstr, doc);
}

void yaml_write_stream(std::ostream &stream, YAML::Emitter const &out)
{
    stream << out.c_str();
}

void yaml_write_file(boost::filesystem::path const &filePath, YAML::Emitter const &out)
{
    std::ofstream fout(filePath.string().c_str());
    yaml_write_stream(fout, out);
    fout.close();
}

void yaml_write_string(std::string &str, YAML::Emitter const &out)
{
    std::ostringstream ss;
    yaml_write_stream(ss, out);
    str = ss.str();
}

void yaml_read_stdin(Node &doc, bool ignoreFirst)
{
    if (ignoreFirst)
        cin.ignore();
    stringbuf buffer;
    cin.get(buffer);

    istream line(&buffer);
    Parser parser(line);
    parser.GetNextDocument(doc);
}

} // end namespace common


namespace YAML
{

void operator>>(const Node &in, boost::filesystem::path &path)
{
    string str;
    in >> str;
    path = str;
}

Emitter &operator<<(Emitter &out, const boost::filesystem::path &path)
{
    return out << path.string();
}

void operator>>(const Node &in, yaml_utilities::YamlLoadable &obj)
{
    obj.load(in);
}

Emitter &operator<<(Emitter &out, const yaml_utilities::YamlDumpable &obj)
{
    obj.dump(out);
    return out;
}

} // namespace YAML


std::ostream &operator<<(std::ostream &os, const YAML::Emitter &out)
{
    return os << out.c_str();
}

std::ostream &operator<<(std::ostream &os, const yaml_utilities::YamlDumpable &obj)
{
    Emitter out;
    out << obj;
    return os << out;
}

#endif // _YAML_UTILITIES_YAML_UTILITIES_H
