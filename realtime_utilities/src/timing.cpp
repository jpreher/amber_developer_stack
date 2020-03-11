/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <sched.h>
#include <stdexcept>

#include <realtime_utilities/timing.hpp>

namespace realtime_utilities
{

bool enable_realtime(bool throw_if_error)
{
    // Following from example real-time code
    sched_param param;
    int policy = SCHED_FIFO;
    param.sched_priority = sched_get_priority_max(policy);
    if (sched_setscheduler(0, policy, &param) == -1)
    {
        if (throw_if_error)
            throw std::runtime_error("sched_setscheduler failed");
        else
            return false;
    }
    else
        return true;
}

} // namespace realtime_utilities
