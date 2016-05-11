/// __process_impl.cpp
#include <cconsole/detail/__first_line_of.hpp>
#include <cmonitor/detail/__process.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <system_error>
#include <stdexcept>
#include <fstream>
#include <cstdlib>

#include <chrono>
#include <thread>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


void crf::proc::detail::__handle_forked(pid_t pid, std::string const& cmdline) {
  if( pid == 0 ) //we are in child..
  {
    ::execlp(cmdline.c_str(), cmdline.c_str(), nullptr);
    ///see: man 2 execve
    ///see: man vfork
    std::_Exit(2);
  }
  else if( pid == -1 )
    throw std::system_error{errno, std::system_category()};

  if (not detail::__has_started(pid))
    throw std::system_error{ECHILD, std::system_category()};
}

bool crf::proc::detail::__has_started(pid_t pid) noexcept {
  static auto _proc_path_format = boost::format{"/proc/%1%"};
  using namespace boost::filesystem;

  auto const path = ( _proc_path_format % pid ).str();
  return( exists(path) and is_directory(path) );
}

pid_t crf::proc::start(std::string const& cmdline) {
  auto const pid = ::vfork();
  crf::proc::detail::__handle_forked(pid, cmdline);
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

  ///see: man 3 kill
  auto const retval = ::kill(pid, SIGKILL);
  if( retval == -1 )
    throw std::system_error{errno, std::system_category()};

  auto status = 0;
  ::waitpid(pid, &status, 0);
}

bool crf::proc::is_alive(pid_t pid, std::string cmdline) noexcept {
  auto const cmdline_p = detail::__cmdline_path_of(pid);
  if (cmdline_p.empty())
    return false;

  std::ifstream ifs{cmdline_p};
  auto const cmdln = crf::first_line_of(ifs);
  return( cmdline == cmdln );
}

