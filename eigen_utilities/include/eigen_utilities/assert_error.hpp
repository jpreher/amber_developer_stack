/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef EIGEN_UTILITIES_COMMON_ASSERT_H_
    #define EIGEN_UTILITIES_COMMON_ASSERT_H_

#include <common_assert/common_assert.hpp>

namespace eigen_utilities
{

/**
 * @brief Specialized assert_error
 * @note To be used in common_assert_msg_ex(expr, msg, eigen_utilities::assert_error)
 */
class assert_error : public common::assert_error
{
public:
    inline assert_error(const std::string &message)
        : common::assert_error(message)
    { }
};

} // eigen_utilities

#endif // EIGEN_UTILITIES_COMMON_ASSERT_H_
