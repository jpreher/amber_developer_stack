/**
 * @brief Unittests for basic YAML binary stuff
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <common_assert/common_assert.hpp>
#include <stl_utilities/container_utilities.hpp>
#include <yaml_utilities/yaml_utilities.hpp>
#include <yaml_utilities/binary_utilities.hpp>

using namespace std;
using namespace YAML;
using namespace yaml_utilities;

/**
 * @brief BasicRead Read a list of strings encoded as a YAML string
 */
TEST(binary_utilities, BasicRead)
{
    {
        int x_expected = 43;
        Emitter out;
        yaml_write_binary(out, x_expected);
        
        int x_actual;
        Node node;
        yaml_read_string(out.c_str(), node);
        yaml_read_binary(node, x_actual);
        
        EXPECT_EQ(x_expected, x_actual);
    }
    
    {
        int x_expected = 22;
        Emitter out;
        yaml_write_binary_dual(out, x_expected);
        
        int x_actual;
        Node node;
        yaml_read_string(out.c_str(), node);
        yaml_read_binary_dual(node, x_actual);
        
        EXPECT_EQ(x_expected, x_actual);
    }
}
