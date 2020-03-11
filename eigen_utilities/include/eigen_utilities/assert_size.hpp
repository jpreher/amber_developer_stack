/**
 * @brief Provide simple macros for checking the size of a matrix and giving informative errors
 * @note Currently uses common_assert_msg(). Could refactor to use a normal assert
 * @note Should be included after <runtime_config.hpp> !!!
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */

#ifndef EIGEN_UTILITIES_ASSERT_SIZE_H_
    #define EIGEN_UTILITIES_ASSERT_SIZE_H_

#include <Eigen/Dense>
#include <eigen_utilities/assert_error.hpp>

namespace eigen_utilities
{

#define assert_size_matrix(X, rows_expected, cols_expected) \
    { \
        common_assert_msg_ex( \
            X.rows() == rows_expected && X.cols() == cols_expected, \
            "matrix [row, col] mismatch" << std::endl << \
                "actual: [" << X.rows() << ", " << X.cols() << "]" << std::endl << \
                "expected: [" << rows_expected << ", " << cols_expected << "]", \
            eigen_utilities::assert_error); \
    }

#define assert_size_vector(X, size_expected) \
    { \
        common_assert_msg_ex( \
            X.size() == size_expected, \
            "matrix size mismatch" << std::endl << \
                "actual: " << X.size() << std::endl << \
                "expected: " << size_expected, \
            eigen_utilities::assert_error); \
    }

    template<typename Derived>
void assert_size(const Eigen::MatrixBase<Derived> &X, int rows_expected, int cols_expected)
{
    common_assert_msg_ex(
            X.rows() == rows_expected && X.cols() == cols_expected,
            "matrix [row, col] mismatch" << std::endl << 
                "actual: [" << X.rows() << ", " << X.cols() << "]" << std::endl << 
                "expected: [" << rows_expected << ", " << cols_expected << "]",
            eigen_utilities::assert_error);
}

    template<typename Derived>
void assert_size(const Eigen::MatrixBase<Derived> &X, int size_expected)
{
    common_assert_msg_ex(
        X.size() == size_expected,
        "matrix size mismatch" << std::endl << 
            "actual: " << X.size() << std::endl << 
            "expected: " << size_expected,
        eigen_utilities::assert_error);
}

} // namespace eigen_utilities

#endif // EIGEN_UTILITIES_ASSERT_SIZE_H_
