/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron Ames's AMBER Lab
 * @author Jenna Reher <jreher@caltech.edu>, member of Dr. Aaron Ames's AMBER Lab
 */
#ifndef CONTROL_UTILITIES_LIMITS_HPP_
#define CONTROL_UTILITIES_LIMITS_HPP_

#include <cmath>
#include <limits>

#include <common_assert/common_assert.hpp>
#include <ros/time.h>

namespace control_utilities
{

/**
 * @brief clamp Clamp a given value within the bounds of min and max
 * @param value The input value to clamp
 * @param min Lower bound. To disable, set to NAN
 * @param max Upper bound. To disable, set to NAN
 * @param presult If not null, set to the following values: {0: No clamping, -1: Lower bounded, 1: Upper bounded}
 * @precondition If both min and max are used, min <= max
 * @see atrias:1ec359e:software/atrias_control_lib/src/AtriasController.cpp:21, clamp()
 * @return
 */
inline double clamp(double value, double min, double max, int *presult = NULL)
{

    bool use_min = !std::isnan(min);
    bool use_max = !std::isnan(max);

    if (use_min && use_max)
        common_assert(min <= max);

    double clamped = value;
    int result = 0;

    if (!std::isnan(value))
    {
        if (use_min && value < min)
        {
            result = -1;
            clamped = min;
        }
        else if (use_max && value > max)
        {
            result = 1;
            clamped = max;
        }
    }

    if (presult)
        *presult = result;

    return clamped;
}

/**
 * @brief clamp Saturate value within absolute bounds
 * @param value
 * @param max Minimum and maximum value
 * @param presult
 * @return
 */
inline double clamp(double value, double max, int *presult = NULL)
{
    if (!std::isnan(max))
        common_assert(max >= 0);
    return clamp(value, -max, max, presult);
}

/**
 * @brief rate_limit Saturate a value given a velocity and a rate bounds
 * @param prev
 * @param cur
 * @param dt
 * @param rate_min
 * @param rate_max
 * @param presult
 * @see atrias:1ec359e:software/atrias_controllers/asc_rate_limit/src/ASCRateLimit.cpp:34, ASCRateLimit::operator()
 * @return
 */
inline double rate_limit(double prev, double cur, double dt, double rate_min, double rate_max, int *presult = NULL)
{
    if (std::isnan(dt))
        return cur;
    common_assert(dt > 0); // Allow zero dt?
    return clamp(cur, prev + dt * rate_min, prev + dt * rate_max, presult);
}

inline double rate_limit(double prev, double cur, double dt, double rate_max, int *presult = NULL)
{
    if (!std::isnan(rate_max))
        common_assert(rate_max >= 0);
    return rate_limit(prev, cur, dt, -rate_max, rate_max, presult);
}

/**
 * @brief RateLimiter Simple class to store rate bounds and previous values
 */
class RateLimiter
{
    double rate_min_;
    double rate_max_;
    double prev_value_;
    int result_;
public:
    inline RateLimiter()
    {
        setLimits(NAN, NAN);
        reset();
    }
    inline RateLimiter(double rate_min, double rate_max)
    {
        setLimits(rate_min, rate_max);
        reset();
    }

    inline RateLimiter(double rate_max)
    {
        setLimits(rate_max);
        reset();
    }

    inline void setLimits(double rate_max)
    {
        rate_min_ = -rate_max;
        rate_max_ = rate_max;
    }

    inline void setLimits(double rate_min, double rate_max)
    {
        rate_min_ = rate_min;
        rate_max_ = rate_max;
    }

    inline void reset(double cur_value = NAN)
    {
        prev_value_ = cur_value;
        result_ = 0;
    }

    inline double update(double dt, double cur_value)
    {
        prev_value_ = rate_limit(prev_value_, cur_value, dt, rate_min_, rate_max_, &result_);
        return prev_value_;
    }

    inline double getPreviousValue() const
    {
        return prev_value_;
    }

    inline int wasSaturated() const
    {
        return result_;
    }

    inline double getPreviousRateSaturation() const
    {
        if (result_ == 0)
            return NAN;
        else if (result_ == -1)
            return rate_min_;
        else if (result_ == 1)
            return rate_max_;
        else
            throw std::runtime_error("Invalid result? Memory corruption?");
    }
};

/**
 * @brief StampedRateLimiter Store rate bounds and use ros time stamps to dictate periods
 */
class StampedRateLimiter : public RateLimiter
{
    ros::Time prev_time_;
public:
    inline StampedRateLimiter(double rate_min, double rate_max)
        : RateLimiter(rate_min, rate_max)
    { }

    inline double update(const ros::Time &time, double cur_value)
    {
        /// @note Let the nan prev_value take care of initialization
        double value = update(time - prev_time_, cur_value);
        prev_time_ = time;
        return value;
    }

    inline double update(const ros::Duration &period, double cur_value)
    {
        return RateLimiter::update(period.toSec(), cur_value);
    }

    inline void reset(double cur_value = NAN, const ros::Time &time = ros::Time())
    {
        RateLimiter::reset(cur_value);
        prev_time_ = time;
    }

    inline const ros::Time& getPreviousTime() const
    {
        return prev_time_;
    }
};

}

#endif // CONTROL_UTILITIES_LIMITS_HPP_
