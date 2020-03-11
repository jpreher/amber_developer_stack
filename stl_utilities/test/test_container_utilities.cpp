/**
 * @brief Unittests for STL container utilities
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <stl_utilities/container_utilities.hpp>

using std::vector;

TEST(test, Basic)
{
	vector<int> expected;
	expected.push_back(1);
	expected.push_back(2);
	expected.push_back(3);

	vector<int> actual;
	actual << 1 << 2 << 3;
	EXPECT_EQ(expected, actual);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
