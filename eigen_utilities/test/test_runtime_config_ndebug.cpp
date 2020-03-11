/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */

// Disable debugging in this translation unit
#ifndef EIGEN_UTILITIES_NDEBUG
    #define EIGEN_UTILITIES_NDEBUG
#endif

#include <eigen_utilities/runtime_config.hpp>
#include <Eigen/Dense>

// See test_runtime_config for more information

namespace test_runtime_config_ndebug
{

bool resize_matrix_calling_disable()
{
    eigen_utilities::MallocAllowedScope scope(false);
    Eigen::MatrixXd blank;
    blank.resize(5, 5);
    return scope.wasMallocPreviouslyEnabled();
}
    
}
