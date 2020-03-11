/**
 * @author Jenna Reher <jreher@caltech.edu>, member of Dr. Aaron
 * Ames's AMBER Lab
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <control_eigen_utilities/limits.hpp>

using Eigen::VectorXd;
using Eigen::VectorXi;
using namespace control_eigen_utilities;

TEST(limits_test, clamp_values)
{
    // Check a series of values
    int size = 4;
    VectorXd min(size);
    min << -1, -2, -3, -4;
    VectorXd max(size);
    max << 1, 2, 3, 4;
    VectorXd values(size);
    values << -10.5, 0.5, 2.8, 20;
    VectorXi results_expected(size);
    results_expected << -1, 0, 0, 1;
    VectorXd clamped_expected(size);
    clamped_expected << min(0), values(1), values(2), max(3);

    {
        // First ensure that passing null pointers works as expected
        bool was_clamped = control_eigen_utilities::clamp(values, min, max);
        EXPECT_TRUE(was_clamped);
    }

    {
        // Ensure that only getting the result works
        VectorXd clamped;
        bool was_clamped = control_eigen_utilities::clamp(values, min, max, &clamped);
        EXPECT_TRUE(was_clamped);
        EXPECT_EQ(clamped_expected, clamped);
    }

    {
        // Ensure getting all information works
        VectorXd clamped;
        VectorXi results;
        bool was_clamped = control_eigen_utilities::clamp(values, min, max, &clamped, &results);
        EXPECT_TRUE(was_clamped);
        EXPECT_EQ(clamped_expected, clamped);
        EXPECT_EQ(results_expected, results);
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
