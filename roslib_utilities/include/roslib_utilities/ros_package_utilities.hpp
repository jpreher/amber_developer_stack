/**
 * @brief Utility functions for ros/package
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef _COMMON_ROS_PACKAGE_UTILITIES_
    #define _COMMON_ROS_PACKAGE_UTILITIES_

#include <boost/filesystem.hpp>

namespace roslib_utilities
{

/**
 * @brief resolve_local_ros_uri Resolve package:// or file:// to an file path on a local system.
 * @param url The url, which is either a file path, a package path, package://${PACKAGE}/..., or file://${PATH}
 * @return The final file path, which may be relative if a basic file path was used.
 * @note You could pass this output to boost::filesystem::absolute(rel, base=pwd())
 */
boost::filesystem::path resolve_local_url(const boost::filesystem::path &url);

}

#endif // _COMMON_ROS_PACKAGE_UTILITIES_
