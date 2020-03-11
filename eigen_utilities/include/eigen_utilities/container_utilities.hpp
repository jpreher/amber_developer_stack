/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef EIGEN_UTILITIES_CONTAINER_UTILITIES_H
    #define EIGEN_UTILITIES_CONTAINER_UTILITIES_H

#include <Eigen/Dense>

#include <eigen_utilities/assert_size.hpp>

namespace eigen_utilities
{

template<typename DerivedA, typename DerivedB, typename Container>
void select_rows(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(out, indices.size(), in.cols());
    for (int i = 0; i < indices.size(); ++i)
    {
        int ix = indices[i];
        common_assert_msg(ix >= 0 && ix < in.rows(), "ix: " << ix << ", in: " << in.rows());
        out.row(i) = in.row(ix);
    }
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_rows_resize(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    out.derived().resize(indices.size(), in.cols());
    select_rows(in, indices, out);
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_cols(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(out, in.rows(), indices.size());
    for (int i = 0; i < indices.size(); ++i)
    {
        int ix = indices[i];
        common_assert_msg(ix >= 0 && ix < in.cols(), "ix: " << ix << ", in: " << in.cols());
        out.col(i) = in.col(ix);
    }
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_cols_resize(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    out.derived().resize(in.rows(), indices.size());
    select_cols(in, indices, out);
}

template<typename DerivedA, typename DerivedB, typename ContainerA, typename ContainerB>
void select_grid(const Eigen::MatrixBase<DerivedA> &in, const ContainerA &rows, const ContainerB &cols, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(out, rows.size(), cols.size());
    for (int i = 0; i < rows.size(); ++i)
    {
        for (int j = 0; j < cols.size(); ++j)
            out(i, j) = in(rows[i], cols[j]);
    }
}

template<typename DerivedA, typename DerivedB, typename ContainerA, typename ContainerB>
void select_grid_resize(const Eigen::MatrixBase<DerivedA> &in, const ContainerA &rows, const ContainerB &cols, Eigen::MatrixBase<DerivedB> &out)
{
    out.derived().resize(rows.size(), cols.size());
    select_grid(in, rows, cols, out);
}


template<typename DerivedA, typename DerivedB, typename Container>
void select_rows_reverse(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(in, indices.size(), in.cols());
    for (int i = 0; i < indices.size(); ++i)
    {
        int ox = indices[i];
        common_assert_msg(ox >= 0 && ox < out.rows(), "ox: " << ox << ", out: " << out.rows());
        out.row(ox) = in.row(i);
    }
}

template<typename DerivedA, typename DerivedB, typename Container>
void select_cols_reverse(const Eigen::MatrixBase<DerivedA> &in, const Container &indices, Eigen::MatrixBase<DerivedB> &out)
{
    assert_size(in, in.rows(), indices.size());
    for (int i = 0; i < indices.size(); ++i)
    {
        int ox = indices[i];
        common_assert_msg(ox >= 0 && ox < out.cols(), "ox: " << ox << ", out: " << out.cols());
        out.col() = in.col(i);
    }
}


template<typename DerivedA, typename DerivedB, typename ContainerA, typename ContainerB>
void select_rows(const Eigen::MatrixBase<DerivedA> &in, const ContainerA &in_indices, Eigen::MatrixBase<DerivedB> &out, const ContainerB &out_indices)
{
    assert_size_vector(out_indices, in_indices.size());
    for (int i = 0; i < in_indices.size(); ++i)
    {
        int ix = in_indices[i];
        int ox = out_indices[i];
        common_assert_msg(ix >= 0 && ix < in.rows(), "ix: " << ix << ", in: " << in.rows());
        common_assert_msg(ox >= 0 && ox < out.rows(), "ox: " << ox << ", out: " << out.rows());
        out.row(ox) = in.row(ix);
    }
}


template<typename DerivedA, typename DerivedB, typename ContainerA, typename ContainerB>
void select_cols(const Eigen::MatrixBase<DerivedA> &in, const ContainerA &in_indices, Eigen::MatrixBase<DerivedB> &out, const ContainerB &out_indices)
{
    assert_size_vector(out_indices, in_indices.size());
    for (int i = 0; i < in_indices.size(); ++i)
    {
        int ix = in_indices[i];
        int ox = out_indices[i];
        common_assert_msg(ix >= 0 && ix < in.cols(), "ix: " << ix << ", in: " << in.cols());
        common_assert_msg(ox >= 0 && ox < out.cols(), "ox: " << ox << ", out: " << out.cols());
        out.col(ox) = in.col(ix);
    }
}

}

#endif // EIGEN_UTILITIES_CONTAINER_UTILITIES_H
