/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <Eigen/Dense>
#include <eigen_utilities/assert_size.hpp>

TEST(eigen_utilities, basic_check)
{
    Eigen::VectorXd X(10);
    EXPECT_NO_THROW(eigen_utilities::assert_size(X, 10));
    EXPECT_THROW(eigen_utilities::assert_size(X, 8), eigen_utilities::assert_error);
    
    Eigen::MatrixXd A(10, 10);
    EXPECT_NO_THROW(eigen_utilities::assert_size(A, 10, 10));
    EXPECT_THROW(eigen_utilities::assert_size(A, 8, 4), eigen_utilities::assert_error);
    
    // Print out error
    try
    {
        eigen_utilities::assert_size(X, 8, 4);
    }
    catch (eigen_utilities::assert_error &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    // Print out error
    try
    {
        assert_size_matrix(X, 8, 4);
    }
    catch (eigen_utilities::assert_error &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
