/**
 * @brief Unittests for ros/package utilities
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <ros/package.h>
#include <roslib_utilities/ros_package_utilities.hpp>

using std::string;

TEST(test, BasicResolution)
{
    string input = "file:///tmp/something.txt";
    string expected = "/tmp/something.txt";
    string actual = roslib_utilities::resolve_local_url(input).string();
    EXPECT_EQ(expected, actual);
}

TEST(test, ExplicitResolution)
{
    string input = "package://roslib_utilities/CMakeLists.txt";
    string expected = ros::package::getPath("roslib_utilities") + "/CMakeLists.txt";
    string actual = roslib_utilities::resolve_local_url(input).string();
    EXPECT_EQ(expected, actual);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
