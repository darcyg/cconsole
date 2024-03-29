/// __monitor_impl.cpp
#include <cmonitor/detail/__memory_usage.hpp>
#include <cmonitor/detail/__cpu_time.hpp>
#include <cmonitor/detail/__process.hpp>
#include <cmonitor/detail/__monitor.hpp>
#include <boost/filesystem.hpp>
#include <thread>

using crf::proc::monitor;
std::chrono::milliseconds constexpr crf::proc::monitor::_default_parse_interval;

struct crf::proc::monitor::__descriptor {

  /// to obtain cpu total from
  static auto constexpr _stat_file = "/proc/stat";
  static std::string const _stat_path;

  static boost::format _proc_path_format;
  /// to obtain memory consumption data from
  static boost::format  _proc_status_file_format;
  /// to obtain runtime info from -- stime, utime, state
  static boost::format  _proc_stat_file_format;

  __descriptor(pid_t pid_, std::string cmdline_);

  pid_t pid;
  std::string cmdline;
  std::string proc_path;
  std::string proc_stat_path;
  std::string proc_status_path;
};

std::string const monitor::__descriptor::_stat_path{_stat_file};
boost::format monitor::__descriptor::_proc_path_format        = boost::format{"/proc/%1%"};
boost::format monitor::__descriptor::_proc_status_file_format = boost::format{"%1%/status"};
boost::format monitor::__descriptor::_proc_stat_file_format   = boost::format{"%1%/stat"};


crf::proc::monitor::__descriptor::__descriptor(pid_t pid_, std::string cmdline_)
: pid{pid_}
, cmdline{std::move(cmdline_)}
, proc_path{( _proc_path_format % pid ).str()}
, proc_stat_path{( _proc_stat_file_format % proc_path ).str()}
, proc_status_path{( _proc_status_file_format % proc_path ).str()}
{
}

monitor::monitor(pid_t const pid, std::string const& cmdline, bool const is_owner)
: _dsc_ptr{new __descriptor{pid, cmdline}}
, _owner{is_owner}
{
  if( _owner )
    _guard.emplace(_dsc_ptr->pid);
}

monitor::monitor(pid_t const pid, bool const is_owner) : monitor(pid, crf::proc::cmdline_of(pid), is_owner)
{
}

monitor::monitor(pid_t const pid): monitor(pid, false)
{
}

monitor::monitor(std::string const& cmdline): monitor(crf::proc::start(cmdline), true)
{
}

monitor::~monitor() {
  delete _dsc_ptr;
}

monitor::sample_t monitor::sample(std::chrono::milliseconds const parse_interval) const {
  if (! is_process_alive())
    throw std::domain_error{"process is not running"};

  auto const cpu_time_1 = crf::proc::make_cpu_time(__descriptor::_stat_path, _dsc_ptr->proc_stat_path);
  auto const mem_info   = crf::proc::make_memory_info(_dsc_ptr->proc_status_path); 
  std::this_thread::sleep_for(parse_interval);
  auto const cpu_time_2 = crf::proc::make_cpu_time(__descriptor::_stat_path, _dsc_ptr->proc_stat_path);
  auto const usage      = crf::proc::cpu_usage_of(cpu_time_1, cpu_time_2);

  ///TODONE: add number of threads support..
  return { _dsc_ptr->pid
          ,usage
          ,cpu_time_2.utime
          ,cpu_time_2.stime
          ,mem_info.vm_rss
          ,mem_info.vm_size
          ,mem_info.vm_peak };
}

pid_t monitor::pid() const noexcept {
  return _dsc_ptr->pid;
}

std::string const& monitor::cmdline() const noexcept {
  return _dsc_ptr->cmdline;
}

bool monitor::is_owner() const noexcept {
  return _owner;
}

bool monitor::is_process_alive() const noexcept {
  return crf::proc::is_alive(_dsc_ptr->pid, _dsc_ptr->cmdline);
}

monitor::operator bool () const noexcept {
  return( _dsc_ptr->pid > 0 );
}

