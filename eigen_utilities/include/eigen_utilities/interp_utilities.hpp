/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef EIGEN_UTILITIES_INTERP_UTILITIES_H
    #define EIGEN_UTILITIES_INTERP_UTILITIES_H

#include <Eigen/Dense>

#include <vector>
#include <eigen_utilities/assert_size.hpp>

namespace eigen_utilities
{

template<typename T, typename Container>
T find_lower_index(const Container &data, const T &value, bool do_saturate = false)
{
    // Assume sorted
    if (data.size() == 0)
        return -1;
    else
    {
        int index = 0;
        int last = data.size() - 1;
        if (do_saturate)
        {
            const T &min = data[0];
            const T &max = data[last];
            if (value < min)
                return -2;
            if (value > max)
                return -3; // Return this as upper
        }
        while (index + 1 < data.size())
        {
            const T &lower = data[index];
            const T &upper = data[index + 1];
            if (value >= lower && value <= upper)
                return index;
            else
                index += 1;
        }
        return -1;
    }
}

template<typename ZType>
class BilinearInterp
{
public:
    Eigen::VectorXd xs;
    Eigen::VectorXd ys;
    std::vector<std::vector<ZType> > zs;
private:
    ZType q11;
    ZType q21;
    ZType q12;
    ZType q22;
    ZType r1;
    ZType r2;
    ZType z;
public:
    void check()
    {
        common_assert(xs.size() > 0);
        common_assert(ys.size() > 0);
        common_assert_msg(zs.size() == xs.size(), "Bad tensor x-size: " << zs.size() << " != " << xs.size());
        for (int i = 0; i < zs.size(); ++i)
            common_assert_msg(zs[i].size() == ys.size(), "Bad tensor y-size at [" << i << "]: " << zs[i].size() << " != " << ys.size());
    }

    inline const ZType& eval(const Eigen::VectorXd &v, bool do_saturate = false)
    {
        assert_size_vector(v, 2);
        return eval(v(0), v(1), do_saturate);
    }

    inline const ZType& eval(double x, double y, bool do_saturate = false)
    {
//        common_assert(xs.size() > 0);
//        common_assert(ys.size() > 0);
//        assert_size_matrix(zs, xs.size(), ys.size());
        // http://en.wikipedia.org/wiki/Bilinear_interpolation
        if (do_saturate)
        {
            double xmin = xs(0);
            double xmax = xs(xs.size() - 1);
            x = std::min(std::max(x, xmin), xmax);
            double ymin = ys(0);
            double ymax = ys(ys.size() - 1);
            y = std::min(std::max(y, ymin), ymax);
        }
        int x_index = find_lower_index(xs, x);
        int y_index = find_lower_index(ys, y);
        if (x_index == -1 || y_index == -1)
            throw std::runtime_error("Bilinear interpolation values out of range");
        double xl = xs(x_index);
        double xu = xs(x_index + 1);
        double yl = ys(y_index);
        double yu = ys(y_index + 1);
        q11 = zs[x_index][y_index];
        q21 = zs[x_index + 1][y_index];
        q12 = zs[x_index][y_index + 1];
        q22 = zs[x_index + 1][y_index + 1];
        r1 = (xu - x) / (xu - xl) * q11 + (x - xl) / (xu - xl) * q21;
        r2 = (xu - x) / (xu - xl) * q12 + (x - xl) / (xu - xl) * q22;
        z = (yu - y) / (yu - yl) * r1 + (y - yl) / (yu - yl) * r2;
        return z;
    }
};

//class MatrixBilinearInterp
//{
//public:
//    Eigen::VectorXd xs;
//    Eigen::VectorXd ys;
//    std::vector<std::vector<Eigen::MatrixXd> > zs;
//private:
//    Eigen::MatrixXd Z;
//    std::vector<std::vector<BilinearInterp> > interps;
//public:
//    void checkAndInit()
//    {
//        common_assert(xs.size() > 0);
//        common_assert(ys.size() > 0);
//        common_assert_msg(zs.size() == xs.size(), "Bad tensor x-size: " << zs.size() << " != " << xs.size());
//        int rows = -1;
//        int cols = -1;
//        for (int i = 0; i < zs.size(); ++i)
//        {
//            common_assert_msg(zs[i].size() == ys.size(), "Bad tensor y-size at [" << i << "]: " << zs[i].size() << " != " << ys.size());
//            for (int j = 0; j < zs[i].size(); ++j)
//            {
//                if (rows == -1 && cols == -1)
//                    assert_size_matrix(zs[i][j], rows, cols);
//                else
//                {
//                    rows = zs[i][j].rows();
//                    cols = zs[i][j].cols();
//                    Z.resize(rows, cols);
//                }
//            }
//        }
//        std::vector<std::vector<BilinearInterp> > interp_row;
//        Eigen::MatrixXd blank(xs.size(), ys.size());
//        interp_row.assign(cols, BilinearInterp(xs, ys, blank));
//        interps.assign(rows, interp_row);
//        for (int ix = 0; ix < xs.size(); ++ix)
//        {
//            for (int iy = 0; iy < ys.size(); ++iy)
//            {
//                for (int ir = 0; ir < rows; ++ ir)
//                {
//                    for (int ij = 0; ij < cols; ++ ij)
//                    {
//                        interps[ir][ij].zs(ix, iy) = zs[ix][iy](ir, ij);
//                    }
//                }
//            }
//        }
//    }
//};

}

#endif // EIGEN_UTILITIES_INTERP_UTILITIES_H
