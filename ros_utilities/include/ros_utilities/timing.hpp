/**
 * @author Jenna Reher <jreher@caltech.edu>, member of Dr. Aaron Ames's AMBER Lab
 */

#ifndef _ROS_UTILITIES_TIMING_H
#define _ROS_UTILITIES_TIMING_H

#include <stdlib.h>
#include <assert.h>
#include <ros/time.h>


namespace ros_utilities {

class Timer {
public:
    int mode;
    bool allow_continuous;
    double t_start;
    double t_end;

    Timer(bool allow_continuous = false)
        : mode(0), allow_continuous(allow_continuous) {
    }

    void start() {
        assert(mode != 1);
        mode = 1;
        t_start = ros::Time::now().toSec();
    }

    void stop() {
        t_end = ros::Time::now().toSec();
        assert(mode == 1);
        mode = 2;
    }

    void reset() {
        mode = 0;
    }

    void restart() {
        reset();
        start();
    }

    double elapsed() {
        if (mode == 1 && allow_continuous)
        {
            double t_temp = ros::Time::now().toSec();
            return (t_temp - t_start);
        }
        else
        {
            std::cout << "asserted mode 2!" << std::endl;
            assert(mode == 2);
            return (t_end - t_start);
        }
    }
};

}


#endif // _ROS_UTILITIES_TIMING_H
