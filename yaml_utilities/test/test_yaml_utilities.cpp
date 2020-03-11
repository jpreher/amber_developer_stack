/**
 * @brief Unittests for basic YAML stream helper functions
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <common_assert/common_assert.hpp>
#include <stl_utilities/container_utilities.hpp>
#include <yaml_utilities/yaml_utilities.hpp>

using namespace std;
using namespace YAML;
using namespace yaml_utilities;

/**
 * @brief BasicRead Read a list of strings encoded as a YAML string
 */
TEST(yaml_utilities, BasicRead)
{
    Node node;
    yaml_read_string("[a, b, c]", node);
    vector<string> actual, expected;
    node >> actual;
    expected << "a" << "b" << "c";
    EXPECT_EQ(expected, actual);
}

/**
 * @brief BasicWrite Do the reverse
 */
TEST(yaml_utilities, BasicWrite)
{
    string actual, expected = "[a, b, c]";
    vector<string> test;
    test << "a" << "b" << "c";
    Emitter out;
    out << Flow << test;
    yaml_write_string(actual, out);
    EXPECT_EQ(expected, actual);
}

TEST(yaml_utilies, FileWriteAndRead)
{
    vector<string> expected;
    expected << "a" << "b" << "c";
    Emitter out;
    out << Flow << expected;
    yaml_write_file("/tmp/test_yaml_utilities.yaml", out);

    Node node;
    yaml_read_file("/tmp/test_yaml_utilities.yaml", node);
    vector<string> actual;
    node >> actual;
    EXPECT_EQ(expected, actual);
}

/**
 * @brief FileDoesNotExistTest Ensure that an exception is thrown if we try to read a nonexistant file
 */
TEST(yaml_utilities, FileDoesNotExistTest)
{
    Node node;
    boost::filesystem::path badFilePath = "/tmp/I_HOPE_THIS_FILE_DOES_NOT_EXIST_ON_YOUR_SYSTEM_OTHERWISE_THIS_TEST_WILL_BREAK_SHOULD_WE_USE_AN_INVALID_FILE_NAME_INSTEAD";
    EXPECT_THROW(yaml_read_file(badFilePath, node), runtime_error);
}

/**
 * @brief BadDereferenceTest Ensure that we get a YAML::BadDereference exception if reading a null node
 */
TEST(yaml_utilities, BadDereferenceTest)
{
    Node node;
    yaml_read_string("", node);
    EXPECT_THROW(node["something"].to<int>(), BadDereference);
    EXPECT_EQ(NodeType::Null, node.Type());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
