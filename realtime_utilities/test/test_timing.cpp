/**
 * @brief Unittests for basic timing functions
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <realtime_utilities/timing.hpp>

using namespace realtime_utilities;

/**
 * @brief Test timers, ensure that timing is consistent with a monotonic clock
 * Outer loops should take the most time, inner loops take the least.
 * Also shows how to use clock_nanosleep()
 */
TEST(realtime_utilities, TimerExpectations)
{
    uint ns_sleep = 100u;
    Timer outer, inner;
    timespec t_sleep, t_outer_start[2], t_outer_end[2];
    
    // Time
    clock_gettime(CLOCK_MONOTONIC, &t_outer_start[0]);
    clock_gettime(CLOCK_MONOTONIC, &t_outer_start[1]);
    
    u_int64_t nsec_start = clock_nsec();
    
    {
        Timer::Scope scope_outer(outer);
        {
            Timer::Scope scope_inner(inner);
            
            timespec t_sleep;
            clock_gettime(CLOCK_MONOTONIC, &t_sleep);
            add_time_delta(&t_sleep, ns_sleep);
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                    &t_sleep, 0);
        }
    }
    
    u_int64_t nsec_end = clock_nsec();
    
    clock_gettime(CLOCK_MONOTONIC, &t_outer_end[1]);
    clock_gettime(CLOCK_MONOTONIC, &t_outer_end[0]);
    
    // These times should be in decreasing order
    const int count = 5;
    double diffs[count] = {
        clock_diff(t_outer_start[0], t_outer_end[0]),
        clock_diff(t_outer_start[1], t_outer_end[1]),
        (nsec_end - nsec_start) / double(NSEC_PER_SEC),
        outer.elapsed(),
        inner.elapsed()
    };
    
    for (int i = 0; i < count; ++i)
        EXPECT_GT(diffs[i], 0);
    
    // Ensure that they are in descending order
    for (int i = 1; i < count; ++i)
        EXPECT_GT(diffs[i - 1], diffs[i]);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
