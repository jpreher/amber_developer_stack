/**
 * @brief YAML functions for reading Eigen vectors and matrices
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef _YAML_EIGEN_UTILITIES_HPP_
#define _YAML_EIGEN_UTILITIES_HPP_

#include <Eigen/Dense>
#include <common_assert/common_assert.hpp>
#include <yaml_utilities/yaml_utilities.hpp>

// Have to put these here to overload yaml-cpp's implementation for vector in stlnode
namespace YAML
{

// Reading

template<typename Derived>
void eigen_resize_vector(const Node &in, Eigen::MatrixBase<Derived> &X)
{
    // http://eigen.tuxfamily.org/dox/TopicFunctionTakingEigenTypes.html
    // Blech
    if (in.Type() == YAML::NodeType::Scalar)
        X.derived().resize(1);
    else
        X.derived().resize(in.size());
}

template<typename Derived>
void eigen_read_vector(const Node &in, Eigen::MatrixBase<Derived> &X)
{
    if (in.Type() == YAML::NodeType::Scalar)
    {
        common_assert((uint)X.size() == 1);
        in >> X(0);
    }
    else
    {
        common_assert_msg((uint)X.size() == (uint)in.size(), "Size not equal. Eigen = " << X.size() << ", Yaml = " << in.size());
        for (uint i = 0; i < (uint)X.size(); ++i)
            in[i] >> X(i);
    }
}

template<typename Derived>
void eigen_resize_matrix(const Node &in, Eigen::MatrixBase<Derived> &X)
{
    uint rows = in.size();
    if (rows > 0)
    {
        // If just one row, then resize to row vector
        uint cols = in[0].size();
        if (cols == 0)
            X.derived().resize(1, rows);
        else
            X.derived().resize(rows, in[0].size());
    }
    else
        X.derived().resize(0, 0);
}

template<typename Derived>
void eigen_read_matrix(const Node &in, Eigen::MatrixBase<Derived> &X)
{
    typedef Eigen::MatrixBase<Derived> Matrix;
    uint rows = in.size();
    if (rows > 0)
    {
        uint cols = in[0].size();
        // Load as a vector if it's a single list
        if (cols == 0)
            eigen_read_vector(in, X);
        else
        {
            common_assert_msg((uint)X.rows() == rows, "Rows not equal. Eigen = " << X.rows() << ", Yaml = " << rows);
            common_assert_msg((uint)X.cols() == cols, "Cols not equal. Eigen = " << X.cols() << ", Yaml = " << cols);
            for (uint i = 0; i < rows; ++i)
            {
                typename Matrix::RowXpr row = X.row(i);
                eigen_read_vector(in[i], row);
            }
        }
    }
}


// Writing 

template<typename Derived>
void eigen_write_vector(Emitter &out, const Eigen::MatrixBase<Derived> &X)
{
    out << Flow << BeginSeq;
    for (uint i = 0; i < (uint)X.size(); ++i)
        out << X.coeff(i);
    out << EndSeq;
}

template<typename Derived>
void eigen_write_matrix(Emitter &out, const Eigen::MatrixBase<Derived> &X)
{
    out << BeginSeq;
    for (uint i = 0; i < (uint)X.rows(); ++i)
        eigen_write_vector(out, X.row(i));
    out << EndSeq;
}

// Piecing it together

template<typename Derived, int IsVector>
struct eigen_handler_impl
{ };

// Vector specialization
template<typename Derived>
struct eigen_handler_impl<Derived, 1>
{
    typedef Eigen::MatrixBase<Derived> Matrix;
    static void resize(const Node &in, Matrix &X)
    {
        eigen_resize_vector(in, X);
    }
    
    static void read(const Node &in, Matrix &X)
    {
        eigen_read_vector(in, X);
    }
    
    static void write(Emitter &out, const Matrix &X)
    {
        eigen_write_vector(out, X);
    }
};

// Matrix specialization
template<typename Derived>
struct eigen_handler_impl<Derived, 0>
{
    typedef Eigen::MatrixBase<Derived> Matrix;
    static void resize(const Node &in, Matrix &X)
    {
        eigen_resize_matrix(in, X);
    }
    
    static void read(const Node &in, Matrix &X)
    {
        eigen_read_matrix(in, X);
    }
    
    static void write(Emitter &out, const Matrix &X)
    {
        eigen_write_matrix(out, X);
    }
};

// Make it easy to select
template<typename Derived>
struct eigen_handler : public eigen_handler_impl<Derived, Eigen::MatrixBase<Derived>::IsVectorAtCompileTime>
{ };

template<typename Derived>
void eigen_read_and_resize(const Node &in, Eigen::MatrixBase<Derived> &X)
{
    typedef eigen_handler<Derived> handler;
    if (handler::Matrix::SizeAtCompileTime == Eigen::Dynamic)
        handler::resize(in, X);
    handler::read(in, X);
}


// YAML has trouble with the templating stuff, just going to overload some specific matrix here
// @todo Look into this: http://code.google.com/p/yaml-cpp/issues/detail?id=150#c13
template<typename Scalar, int Cols, int Rows>
void operator>>(const Node &in, Eigen::Matrix<Scalar, Cols, Rows> &X)
{
    eigen_read_and_resize(in, X);
}

template<typename Derived>
Emitter &operator<<(Emitter &out, const Eigen::MatrixBase<Derived> &X)
{
    typedef eigen_handler<Derived> handler;
    handler::write(out, X);
    return out;
}

//template <typename Derived>
//inline bool Node::Read(Eigen::MatrixBase<Derived>& value) const
//{
//    eigen_read_and_resize(*this, value);
//}

//template<typename Derived>
//struct convert<Eigen::MatrixBase<Derived> >
//{
//    typedef Eigen::MatrixBase<Derived> Matrix;
//    static Node encode(const Matrix& rhs) {
//        Node node;
//        eigen_handler<Derived>::write(node);
//        return node;
//    }

//    static bool decode(const Node& node, const Eigen::MatrixBase<Derived>& rhs) {
//        eigen_read_and_resize(node, rhs);
//        return true;
//    }
//};

} // End namespace YAML

#endif
