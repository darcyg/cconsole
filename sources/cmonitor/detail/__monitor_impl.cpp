/// __monitor_impl.cpp
#include <cmonitor/detail/__cpu_time.hpp>
#include <cmonitor/detail/__monitor.hpp>
#include <thread>

using crf::proc::monitor;
std::string const monitor::_stat_path = std::string{monitor::_stat_file};
boost::format monitor::_proc_status_file_format = boost::format{"/proc/%1%/status"};
boost::format monitor::_proc_stat_file_format   = boost::format{"/proc/%1%/stat"};

monitor::monitor(pid_t const pid)
: _pid{pid}
, _proc_stat_path{( _proc_stat_file_format % _pid ).str()}
, _proc_status_path{( _proc_status_file_format % _pid ).str()}
{
}

monitor::sample_t monitor::sample(std::chrono::milliseconds const parse_interval) const {
 auto const cpu_info_1 = crf::proc::make_cpu_time( _stat_path, _proc_stat_path );
 std::this_thread::sleep_for(parse_interval);
 auto const cpu_info_2 = crf::proc::make_cpu_time( _stat_path, _proc_stat_path );
 ///TODONE: collect mem info
 return {};
}

