/**
 * @file throttle_action.hpp
 * @brief Simple definition, takes a line of code and performs its action at a specific rate. Compares time against ros wall clock.
 *
 * @author E. A. Cousineau
 */

#ifndef THROTTLE_ACTION_HPP_
    #define THROTTLE_ACTION_HPP_

#include <iostream>
#include <ros/time.h>

/**
 * @brief Quick hack to perform arbitrary code sinec ROS_CONSOLE does not seem to be threadsafe? Or something...
 * @see Stolen from ROS_LOG_THROTTLE(...)
 */
#define THROTTLE_ACTION(rate, code) \
    do \
    { \
      static double last_hit = 0.0; \
      ::ros::Time now = ::ros::Time::now(); \
      if (last_hit + rate <= now.toSec()) \
      { \
        last_hit = now.toSec(); \
        code; \
        std::cout.flush(); \
        std::cerr.flush(); \
      } \
    } while(0)

#endif // THROTTLE_ACTION_HPP_
