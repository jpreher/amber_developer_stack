/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <vector>
#include <eigen_utilities/interp_utilities.hpp>

using namespace Eigen;
using namespace eigen_utilities;

TEST(eigen_utilities_test, basic_interp)
{
    Eigen::VectorXd xs(2);
    xs << 0, 1;
    Eigen::VectorXd ys(3);
    ys << 0, 1, 2;
    std::vector<std::vector<double> > zs = {{0, 1, 2}, {3, 4, 5}};

    {
        EXPECT_EQ(0, find_lower_index(xs, 0));
        EXPECT_EQ(-1, find_lower_index(xs, -1));
        EXPECT_EQ(-1, find_lower_index(xs, 3));
        EXPECT_EQ(1, find_lower_index(ys, 1.5));
        EXPECT_EQ(1, find_lower_index(ys, 2));

    //    EXPECT_NO_THROW({
        BilinearInterp<double> interp;
        interp.xs = xs;
        interp.ys = ys;
        interp.zs = zs;
        EXPECT_EQ(0, interp.eval(0, 0));
        EXPECT_EQ(5, interp.eval(1, 2));
        EXPECT_EQ(5, interp.eval(5, 5, true));
        EXPECT_EQ(1.5, interp.eval(0.5, 0));
        EXPECT_EQ(0.5, interp.eval(0, 0.5));
        EXPECT_THROW(interp.eval(-1, -1), std::runtime_error);
    }

    // Inerpolate matrices
    {
        std::vector<std::vector<Eigen::MatrixXd> > zs;
        Eigen::MatrixXd A(2, 2);
        A << 0, 1, 2, 3;
        for (int i = 0; i < 2; ++i)
        {
            std::vector<Eigen::MatrixXd> row;
            for (int j = 0; j < 3; ++j)
            {
                int k = j + 3 * i;
                row.push_back(k * A);
            }
            zs.push_back(row);
        }

        BilinearInterp<Eigen::MatrixXd> interp;
        interp.xs = xs;
        interp.ys = ys;
        interp.zs = zs;
        EXPECT_EQ(0 * A, interp.eval(0, 0));
        EXPECT_EQ(5 * A, interp.eval(1, 2));
        EXPECT_NE(6 * A, interp.eval(1, 2));
        EXPECT_EQ(5 * A, interp.eval(5, 5, true));
        EXPECT_EQ(1.5 * A, interp.eval(0.5, 0));
        EXPECT_EQ(0.5 * A, interp.eval(0, 0.5));
    }
}
