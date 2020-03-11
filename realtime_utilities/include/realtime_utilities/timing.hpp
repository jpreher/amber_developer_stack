/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef _REALTIME_UTILITIES_TIMING_H
    #define _REALTIME_UTILITIES_TIMING_H

#include <stdlib.h>
#include <time.h>
#include <assert.h>

namespace realtime_utilities
{

const int NSEC_PER_SEC = 1000000000;

/**
 * @brief clock_diff Compute the difference between two timespecs
 * @param t_start Start time
 * @param t_end End time
 * @return Difference in seconds
 */
inline double clock_diff(const timespec &t_start, const timespec &t_end)
{
    long long diff = NSEC_PER_SEC * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_nsec - t_start.tv_nsec);
    return diff / double(NSEC_PER_SEC);
}

/**
 * @brief Return monotonic clock in nanoseconds
 * @return 
 */
inline u_int64_t clock_nsec()
{
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return NSEC_PER_SEC * t.tv_sec + t.tv_nsec;
}

/**
 * @brief Elevate process priority to real-time status
 * @note Adapted from other examples
 * @param throw_if_error Throw an error if unable to set realtime priority
 * @return True if realtime enabled, false if otherwise
 */
bool enable_realtime(bool throw_if_error = true);

/**
 * @brief Add a time delta to an existing timespec
 * @param t
 * @param dt
 * @note From Simulink generated code
 */
inline void add_time_delta(struct timespec *t, unsigned int dt)
{
    t->tv_nsec += dt;
    while (t->tv_nsec >= NSEC_PER_SEC) {
        t->tv_nsec -= NSEC_PER_SEC;
        t->tv_sec++;
    }
}

/**
 * @brief Timer Measuring time with a monotonic clock and nanosecond resolution
 * @author Eric Cousineau <eacousineau@gmail.com>
 */
struct Timer
{
private:
    /** @brief Start time */
    timespec t_start;
    
    /** @brief End time */
    timespec t_end;
    
    /** @brief Mode the timer is in: {0: Not started, 1: Started, 2: Stopped} */
    int8_t mode;
    
    /** @brief Allow elapsed() to be called even if the timer is not stopped */
    bool allow_continuous;
public:
    Timer(bool allow_continuous = false)
        : mode(0), allow_continuous(allow_continuous)
    { }
    
    /** @brief Start the timer */
    inline void start()
    {
        assert(mode != 1);
        mode = 1;
        clock_gettime(CLOCK_MONOTONIC, &t_start);
    }
    
    /** @brief Zero the initial time */
    inline void start_zero()
    {
        assert(mode != 1);
        mode = 1;
        t_start.tv_nsec = 0;
        t_start.tv_sec = 0;
    }
    
    /** @brief End the timer */
    inline void stop()
    {
        clock_gettime(CLOCK_MONOTONIC, &t_end);
        assert(mode == 1);
        mode = 2;
    }
    
    /** @brief Reset the timer */
    inline void reset()
    {
        mode = 0;
    }

    /** @brief Restart timer */
    inline void restart()
    {
        reset();
        start();
    }
    
    /**
     * @brief elapsed Get the elapsed time
     * @return Elapsed time in seconds using clock_diff()
     */
    inline double elapsed() const
    {
        if (mode == 1 && allow_continuous)
        {
            timespec t_temp;
            clock_gettime(CLOCK_MONOTONIC, &t_temp);
            return clock_diff(t_start, t_temp);
        }
        else
        {
            assert(mode == 2);
            return clock_diff(t_start, t_end);
        }
    }
    
    /** @brief Scope Scoping mechanism to automatically start / stop a timer */
    class Scope
    {
    private:
        /** @brief Reference to timer being started and stopped */
        Timer &timer;
    public:
        inline Scope(Timer &timer)
            : timer(timer)
        {
            timer.start();
        }
        inline ~Scope()
        {
            timer.stop();
        }
    };
};

}

#endif // _REALTIME_UTILITIES_TIMING_H
