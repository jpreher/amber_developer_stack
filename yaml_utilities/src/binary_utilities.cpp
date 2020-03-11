/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include "yaml_utilities/binary_utilities.hpp"

using namespace std;
using namespace YAML;

namespace yaml_utilities
{

static const char encoding[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string encode_base64(const unsigned char *data, std::size_t size)
{
    const char PAD = '=';
   
    std::string ret;
    ret.resize(4 * size / 3 + 3);
    char *out = &ret[0];
   
    std::size_t chunks = size / 3;
    std::size_t remainder = size % 3;
   
    for(std::size_t i=0;i<chunks;i++, data += 3) {
        *out++ = encoding[data[0] >> 2];
        *out++ = encoding[((data[0] & 0x3) << 4) | (data[1] >> 4)];
        *out++ = encoding[((data[1] & 0xf) << 2) | (data[2] >> 6)];
        *out++ = encoding[data[2] & 0x3f];
    }
   
    switch(remainder) {
        case 0:
            break;
        case 1:
            *out++ = encoding[data[0] >> 2];
            *out++ = encoding[((data[0] & 0x3) << 4)];
            *out++ = PAD;
            *out++ = PAD;
            break;
        case 2:
            *out++ = encoding[data[0] >> 2];
            *out++ = encoding[((data[0] & 0x3) << 4) | (data[1] >> 4)];
            *out++ = encoding[((data[1] & 0xf) << 2)];
            *out++ = PAD;
            break;
    }
   
    ret.resize(out - &ret[0]);
    return ret;
}

static const unsigned char decoding[] = {
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 63,
     52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,  0,255,255,
    255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255,
    255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
};

void decode_base64(const std::string& input, std::vector<unsigned char> &ret)
{
    typedef std::vector<unsigned char> ret_type;
    ret.clear();
    if(input.empty())
        return;
   
    ret.resize(3 * input.size() / 4 + 1);
    unsigned char *out = &ret[0];
   
    unsigned value = 0;
    for(std::size_t i=0;i<input.size();i++) {
        unsigned char d = decoding[static_cast<unsigned>(input[i])];
        if(d == 255)
        {
            ret.clear();
            return;
        }

        value = (value << 6) | d;
        if(i % 4 == 3) {
            *out++ = value >> 16;
            if(i > 0 && input[i - 1] != '=')
                *out++ = value >> 8;
            if(input[i] != '=')
                *out++ = value;
        }
    }
   
    ret.resize(out - &ret[0]);
}

void yaml_read_binary(const Node &node, std::vector<unsigned char> &data)
{
    string tmp;
    node >> tmp;
    decode_base64(tmp, data);
}

void yaml_read_binary(const Node &node, void *data, size_t size)
{
    std::vector<unsigned char> buf;
    yaml_read_binary(node, buf);
    assert(buf.size() == size);
    memcpy(data, buf.data(), size);
}

void yaml_write_binary(Emitter &out, const void *data, size_t size)
{
    out << encode_base64((unsigned char*)data, size);
}

} // yaml_utilities
