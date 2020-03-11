/**
 * @brief Unittests to show usage for and ensure proper function of assertion utility
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <common_assert/common_assert.hpp>

TEST(test, NoError)
{
    EXPECT_NO_THROW(common_assert(true));
}

TEST(test, Error)
{
    EXPECT_THROW(common_assert(false), common::assert_error);
}

TEST(test, ErrorWithMessage)
{
    int count = 10;
    int max = 5;
    std::string message;
    // Slurp message from the exception
    EXPECT_THROW(
        try
        {
            common_assert_msg(count < max, "count: " << count << ", max: " << max)
        }
        catch (const common::assert_error &e)
        {
            message = e.what();
            throw e;
        }
    , common::assert_error);
    if (!message.empty())
    {
        std::cout << "Assertion Message:" << std::endl << message << std::endl;
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
