/**
 * @brief Add a simple implementation of reading binary for yaml-cpp 0.2.7 (with ros-groovy)
 * @ref https://code.google.com/p/yaml-cpp/source/browse/src/binary.cpp?r=53943daa27837eb61e9cc1a74fe551285d0c6840
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef _YAML_UTILITIES_BINARY_UTILITIES_H
    #define _YAML_UTILITIES_BINARY_UTILITIES_H

#include <string>
#include <vector>
#include <yaml_utilities/yaml_utilities.hpp>

namespace yaml_utilities
{

/**
 * @brief encode_base64 Encode a set of bytes to Base 64
 * @param data
 * @param size
 * @return 
 */
std::string encode_base64(const unsigned char *data, std::size_t size);

/**
 * @brief decode_base64 Decode a set of bytes from Base 64 to a string
 * @param input
 * @param ret
 */
void decode_base64(const std::string& input, std::vector<unsigned char> &ret);

/**
 * @brief yaml_read_binary Read a string and interpret as a Base 64 array of bytes
 * @param node
 * @param data
 */
void yaml_read_binary(const YAML::Node &node, std::vector<unsigned char> &data);

/**
 * @brief yaml_read_binary  Read a string and interpret as a Base 64 array of bytes
 * @param node
 * @param data
 * @param size
 */
void yaml_read_binary(const YAML::Node &node, void *data, size_t size);

/**
 * @brief yaml_read_binary Read a string and interpret as a binary form of a data type
 */
    template<typename T>
void yaml_read_binary(const YAML::Node &node, T &data)
{
    yaml_read_binary(node, &data, sizeof(T));
}

template<typename T>
void yaml_read_binary(const YAML::Node &node, std::vector<T> &data)
{
    data.resize(node.size());
    for (int i = 0; i < node.size(); ++i)
        yaml_read_binary(node[i], data[i]);
}

/**
 * @brief yaml_read_binary_dual Read a 2-tuple of an actual representation (for human reading) and binary (for machine reading)
 */
    template<typename T>
void yaml_read_binary_dual(const YAML::Node &node, T &data)
{
    // Read the binary base64 portion
    yaml_read_binary(node["base64"], data);
}

void yaml_write_binary(YAML::Emitter &out, const void *data, size_t size);

    template<typename T>
void yaml_write_binary(YAML::Emitter &out, const T &data)
{
    out << encode_base64((unsigned char*)&data, sizeof(data));
}

template<typename T>
void yaml_write_binary(YAML::Emitter &out, const std::vector<T> &data)
{
    // Write a tuple of dual-representation
    out << YAML::BeginSeq;
    for (int i = 0; i < data.size(); ++i)
        yaml_write_binary(out, data[i]);
    out << YAML::EndSeq;
}

    template<typename T>
void yaml_write_binary_dual(YAML::Emitter &out, const T &data)
{
    // Write a tuple of dual-representation
    out << YAML::BeginMap
        << YAML::Key << "value" << YAML::Value << data
        << YAML::Key << "base64" << YAML::Value;
    yaml_write_binary(out, data);
    out << YAML::EndMap;
}


}

#endif // _YAML_UTILITIES_BINARY_UTILITIES_H

