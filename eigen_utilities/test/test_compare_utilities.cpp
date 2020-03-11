/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <eigen_utilities/compare_utilities.hpp>

using namespace Eigen;
using namespace eigen_utilities;

TEST(eigen_utilities_test, hasnan)
{
    MatrixXd X(2, 2);
    X <<
        0, 1,
        2, 3;
    EXPECT_FALSE(hasnan(X));
    EXPECT_FALSE(isnan(X));
    
    X(0) = NAN;
    EXPECT_TRUE(hasnan(X));
    EXPECT_FALSE(isnan(X));
    
    X.setConstant(NAN);
    EXPECT_TRUE(hasnan(X));
    EXPECT_TRUE(isnan(X));
}

TEST(eigen_utilities_test, nan_compare)
{
    
    // Scalars
    EXPECT_TRUE(nan_compare(NAN, NAN));
    EXPECT_TRUE(nan_compare(0, 0));
    EXPECT_FALSE(nan_compare(0, NAN));
    EXPECT_FALSE(nan_compare(NAN, 0));
    EXPECT_EQ(1., norm_nanless(-1));
    EXPECT_EQ(0., norm_nanless(NAN));
    
    // purpose: Set all NAN entries to zero
    MatrixXd X(3, 3);
    X <<
        1, 2, NAN,
        4, NAN, 6,
        7, 9, NAN;
    
    MatrixXd Y(3, 3);
    Y <<
        NAN, 2, 3,
        NAN, 5, 6,
        7, NAN, 9;
    
    EXPECT_TRUE(nan_compare(X, X));
    EXPECT_FALSE(nan_compare(X, Y));
    EXPECT_TRUE(nan_compare(Y, Y));
}

TEST(eigen_utilities_test, diff_relative_nonzero)
{
    double tolerance = 1e-6;
    double perturb = 1e-8;
    
    MatrixXd A(3, 3);
    A <<
        0, 1, 2e23,
        3, 4, 5,
        6, NAN, perturb;
    
    MatrixXd B(3, 3);
    B <<
        perturb, 1 - perturb, 2e23 * (1 + perturb),
        3, 4, 5,
        6, NAN, 0;
    MatrixXd C = diff_relative_nonzero(A, B);
    
    EXPECT_TRUE(nan_compare(A, B));
    EXPECT_LT(norm_nanless(C), tolerance); // expect to be small
    EXPECT_LT(diff_relative_nonzero(A(0, 0), B(0, 0)), tolerance);
}

TEST(eigen_utilities_test, set_nans)
{
    MatrixXd A(2, 2);
    A <<
         1, 2,
         NAN, 4;
    MatrixXd A_expected(2, 2);
    A_expected <<
         1, 2,
         0, 4;
    set_nans(A);
    EXPECT_EQ(A_expected, A);

    VectorXd b(2);
    b <<
         5, NAN;
    VectorXd b_expected(2);
    b_expected <<
        5, 8;
    set_nans(b, 8);
    EXPECT_EQ(b_expected, b);
}
