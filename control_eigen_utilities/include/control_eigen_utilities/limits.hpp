/**
 * @brief Control utilities for use with Eigen library
 * @author Jenna Reher <jreher@caltech.edu>, member of Dr. Aaron
 * Ames's AMBER Lab
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef CONTROL_EIGEN_UTILITIES_LIMITS_HPP_
    #define CONTROL_EIGEN_UTILITIES_LIMITS_HPP_

#include <Eigen/Dense>
#include <eigen_utilities/assert_size.hpp>
#include <control_utilities/limits.hpp>

namespace control_eigen_utilities
{

/**
 * @brief clamp Clamp a vector-valued function
 * @param value
 * @param min
 * @param max
 * @param pclamped
 * @param presult
 * @return
 */
inline bool clamp(const Eigen::VectorXd &value, const Eigen::VectorXd &min, const Eigen::VectorXd &max, Eigen::VectorXd *pclamped = NULL, Eigen::VectorXi *presult = NULL)
{
    int size = value.size();
    eigen_utilities::assert_size(min, size);
    eigen_utilities::assert_size(max, size);
    if (pclamped)
        pclamped->resize(size);
    if (presult)
        presult->resize(size);

    bool is_clamped = false;
    for (int i = 0; i < size; ++i)
    {
        int result;
        double clamped = control_utilities::clamp(value.coeff(i), min.coeff(i), max.coeff(i), &result);
        if (pclamped)
            pclamped->coeffRef(i) = clamped;
        if (presult)
            presult->coeffRef(i) = result;
        if (result != 0)
            is_clamped = true;
    }
    return is_clamped;
}

}

#endif // CONTROL_EIGEN_UTILITIES_LIMITS_HPP_
