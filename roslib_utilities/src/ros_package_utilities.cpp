/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <ros/package.h>

#include "roslib_utilities/ros_package_utilities.hpp"

namespace roslib_utilities
{

boost::filesystem::path resolve_local_url(const boost::filesystem::path &url)
{
    /// @note Shamelessly filched from resource_retriever/src/retriever.cpp
    const std::string &s_url = url.string();
    std::string mod_url = s_url;
    if (s_url.find("package://") == 0)
    {
        mod_url.erase(0, strlen("package://"));
        size_t pos = mod_url.find("/");
        if (pos == std::string::npos)
        {
            throw std::runtime_error(s_url + ": Could not parse package:// format into file:// format");
        }
        
        std::string package = mod_url.substr(0, pos);
        mod_url.erase(0, pos);
        std::string package_path = ros::package::getPath(package);
        
        if (package_path.empty())
        {
            throw std::runtime_error(s_url + ": Package [" + package + "] does not exist");
        }
        
        return package_path + mod_url;
    }
    else if (s_url.find("file://") == 0)
    {
        mod_url.erase(0, strlen("file://"));
        return mod_url;
    }
    else
        return mod_url;
}

} // namespace common
