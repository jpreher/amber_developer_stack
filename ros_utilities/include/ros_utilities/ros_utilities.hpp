#ifndef AMBER_ROS_UTILITIES_HPP_
    #define AMBER_ROS_UTILITIES_HPP_

#include <ros/ros.h>
#include <vector>
#include <yaml_eigen_utilities/yaml_eigen_utilities.hpp>

namespace ros_utilities
{

inline void ros_quick_init(const std::string &name = "durus_simulink")
{
    /// @see proxi_walking_stack:547bcbf:proxi_simulink/src/walking_hzd.cpp:870
    if (!ros::isInitialized())
    {
        int argc = 0;
        ros::init(argc, NULL, name, ros::init_options::NoSigintHandler);
    }
    if (!ros::isStarted())
        ros::Time::init();
}

template<typename T>
inline std::ostream& operator<<(std::ostream &os, const std::vector<T> &value)
{
    using namespace YAML;
    Emitter out;
    out << Flow << value;
    return os << out.c_str();
}


#define CHECK_STR(x) if (str == #x) return x;
#define TO_STR(x) case x: return #x;

    template<typename A_, typename B_, typename to_, typename from_>
struct Converter
{
    typedef A_ T;
    typedef B_ V;
    typedef to_ to; // B = to(A)
    typedef from_ from; // A = from(B)
};

struct ParamChecker
{
public:
    ros::NodeHandle node;
    bool is_first_check;
    std::string prefix;

    inline void init(const std::string &node_name)
    {
        node = ros::NodeHandle(node_name);
        is_first_check = true;
    }

    ParamChecker()
    {
        is_first_check = true;
    }

    ParamChecker(const std::string &node_name)
    {
        init(node_name);
    }

    ParamChecker(const ParamChecker &parent, const std::string &sub)
    {
        node =  parent.node.getNamespace() + "/" + sub;
        prefix = sub + "/";
        if (!parent.prefix.empty())
            prefix = parent.prefix + prefix;
        is_first_check = parent.is_first_check;
    }

    template<typename T>
    bool checkAndUpdate(const std::string &name, T &var) const
    {
        T temp;
        const char *indent = "    ";
        if (node.getParam(name, temp) && temp != var)
        {
            std::cout << indent << "[update] " << prefix + name << ": " << "{new: " << temp << ", old: " << var << "}\n";
            var = temp;
            return true;
        }
        else
        {
            // Print it out if this is the first ros param check
            if (is_first_check)
            {
                std::cout << indent << "[init] " << prefix + name << ": " << var << "\n";
                return true;
            }
            else
            {
                std::cout << indent << "[same] " << prefix + name << ": " << var << "\n";
                return false;
            }
        }
    }

    template<typename T>
    bool checkAndUpdateYaml(const std::string &name, T &var) const
    {
        YAML::Emitter out;
        out << var;
        std::string temp = out.c_str();
        checkAndUpdate(name, temp);
        yaml_utilities::yaml_read_string(temp, var);
    }

    template<typename C>
    bool checkAndUpdateFormatted(const std::string &name, typename C::A &var) const
    {
        typename C::B temp = C::to(var);
        bool result = checkAndUpdate(name, temp);
        var = C::from(temp);
        return result;
    }

    inline bool endCheck()
    {
        is_first_check = false;
    }
};

}

#endif // AMBER_ROS_UTILITIES_HPP_
