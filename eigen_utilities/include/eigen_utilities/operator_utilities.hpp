/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef EIGEN_OPERATOR_UTILITIES_H
    #define EIGEN_OPERTAOR_UTILITIES_h

#include <Eigen/Dense>

namespace Eigen
{

template<typename Derived>
Eigen::MatrixBase<Derived>& operator-=(Eigen::MatrixBase<Derived> &X, const typename Derived::Scalar &a)
{
    // Acceptably inefficient
    X -= a * Matrix<typename Derived::Scalar, -1, -1>::Ones(X.rows(), X.cols());
    return X;
}

template<typename Derived>
Eigen::MatrixBase<Derived>& operator+=(Eigen::MatrixBase<Derived> &X, const typename Derived::Scalar &a)
{
    X += a * Matrix<typename Derived::Scalar, -1, -1>::Ones(X.rows(), X.cols());
    return X;
}

}

#endif // EIGEN_OPERTAOR_UTILITIES_h
