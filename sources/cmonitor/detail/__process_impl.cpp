/// __process_impl.cpp
#include <cconsole/detail/__first_line_of.hpp>
#include <cmonitor/detail/__process.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <system_error>
#include <stdexcept>
#include <signal.h>
#include <fstream>

#include <chrono>
#include <thread>


pid_t crf::proc::start(std::string const& cmdline) {
  auto const pid = ::fork();
  if( pid == 0 ) //we are in child..
  {
    ::execlp(cmdline.c_str(), cmdline.c_str());
    throw std::system_error{errno, std::system_category()}; 
  }
  return pid;
}

std::string crf::proc::detail::__cmdline_path_of(pid_t pid) noexcept {
  static auto _cmdline_format = boost::format{"/proc/%1%/cmdline"};

  auto const cmdline_p = ( _cmdline_format % pid ).str();
  return boost::filesystem::exists(cmdline_p) ? cmdline_p : "";
}

std::string crf::proc::detail::__stringify(int __errno) {
  static auto _default_format = boost::format{"unhandled error code [%1%] in errno"};
#define __case(ERRNO) case ERRNO: return (#ERRNO);
  switch( __errno )
  {
  __case(E2BIG);
  __case(EACCES);
  __case(EINVAL);
  __case(ELOOP);
  __case(ENAMETOOLONG);
  __case(ENOENT);
  __case(ENOTDIR);

  default: return ( _default_format % __errno ).str();
  }
#undef __case
}

std::string crf::proc::cmdline_of(pid_t pid) {
  static auto _error_message_format = boost::format{"no process is running under pid %1%"};
  auto const cmdline_p = detail::__cmdline_path_of(pid);
  if (cmdline_p.empty())
    throw std::invalid_argument{( _error_message_format % pid ).str()};

  std::ifstream ifs{cmdline_p};
  return crf::first_line_of(ifs);
}

void crf::proc::kill(pid_t pid) {
  if (pid == -1)
    throw std::invalid_argument{"pid: -1 -- cannot issue kill for all processes"};

  ///TODONE: check output and signal error
  ::kill(pid, SIGKILL);
}

bool crf::proc::is_alive(pid_t pid, std::string cmdline) noexcept {
  auto const cmdline_p = detail::__cmdline_path_of(pid);
  if (cmdline_p.empty())
    return false;

  std::ifstream ifs{cmdline_p};
  auto const cmdln = crf::first_line_of(ifs);
  return( cmdline == cmdln );
}

