/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <vector>
#include <eigen_utilities/operator_utilities.hpp>

using namespace Eigen;

TEST(eigen_utilities_test, test_operations)
{
    // Test vectors of integers, subtraction and addition
    {
        Eigen::VectorXi in(4);
        in << 1, 2, 3, 4;
        Eigen::VectorXi expected(4);
        expected << 0, 1, 2, 3;
        Eigen::VectorXi actual = in;
        actual -= 1;
        EXPECT_EQ(expected, actual);
        
        actual = in;
        actual += -1;
        EXPECT_EQ(expected, actual);
    }
    
    // Test matrices of doubles, subtraction and addition
    {
        Eigen::MatrixXd in(2, 2);
        in << 1.5, 2.5, 3.5, 4.5;
        Eigen::MatrixXd expected(2, 2);
        expected << 1, 2, 3, 4;
        Eigen::MatrixXd actual = in;
        actual -= 0.5;
        EXPECT_EQ(expected, actual);
        
        actual = in;
        actual += -0.5;
        EXPECT_EQ(expected, actual);
    }
}
