/// __monitor_impl.cpp
#include <cmonitor/detail/__monitor.hpp>

using crf::proc::monitor;

monitor::monitor(pid_t const pid) noexcept 
: _pid{pid}
{
}

monitor::sample_t monitor::sample() const {
 ///TODONE: impl
 auto const sample = sample_t{};
 return sample;
}

