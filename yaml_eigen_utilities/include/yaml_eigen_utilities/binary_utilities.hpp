/**
  * @brief Quick hack to get binary values
  * @warning MACHINE DEPENDENT!
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
  */
#ifndef _YAML_EIGEN_UTILITIES_BINARY_UTILITIES_HPP_
    #define _YAML_EIGEN_UTILITIES_BINARY_UTILITIES_HPP_

#include <yaml_utilities/binary_utilities.hpp>
#include <yaml_eigen_utilities/yaml_eigen_utilities.hpp>

// Note the namespacing
namespace yaml_utilities
{

    template<>
inline void yaml_write_binary(YAML::Emitter &out, const Eigen::MatrixXd &X)
{
    out << YAML::BeginSeq;
    for (int i = 0; i < X.rows(); ++i)
    {
        out << YAML::Flow << YAML::BeginSeq;
        for (int j = 0; j < X.cols(); ++j)
            yaml_write_binary(out, X.coeff(i, j));
        out << YAML::EndSeq;
    }
    out << YAML::EndSeq;
}

    template<>
inline void yaml_read_binary(const YAML::Node &out, Eigen::MatrixXd &X)
{
    int rows = out.size();
    if (rows > 0)
    {
        int cols = out[0].size();
        X.resize(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                yaml_read_binary(out[i][j], X(i, j));
    }
    else
        X.resize(0, 0);
}

    template<>
inline void yaml_write_binary(YAML::Emitter &out, const Eigen::VectorXd &X)
{
    out << YAML::Flow << YAML::BeginSeq;
    for (int i = 0; i < X.rows(); ++i)
        yaml_write_binary(out, X.coeff(i));
    out << YAML::EndSeq;
}

    template<>
inline void yaml_read_binary(const YAML::Node &out, Eigen::VectorXd &X)
{
    int rows = out.size();
    X.resize(rows);
    for (int i = 0; i < rows; ++i)
        yaml_read_binary(out[i], X(i));
}

}

#endif
