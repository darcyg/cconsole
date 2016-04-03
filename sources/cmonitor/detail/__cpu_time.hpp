/// __cpu_time.hpp
#pragma once
#include <cstdlib>
#include <string>

namespace crf { namespace proc { namespace detail {
struct cpu_time {
  char status;
  std::size_t total;
  std::size_t utime;
  std::size_t stime;
};

struct proc_stat {
  char state;
  std::size_t utime;
  std::size_t stime;
};

std::size_t cpu_total_of(std::string const& cpu_stat_line) __attribute__((flatten));
proc_stat make_proc_stat(std::string const& proc_stat_line) __attribute__((flatten));
cpu_time make_cpu_time(std::string const& cpu_stat_line, std::string const& proc_stat_line) __attribute__((flatten));
} // namespace detail


detail::cpu_time make_cpu_time(std::string const& stat_path, std::string const& proc_stat_path) __attribute__((flatten));
} // namespace proc
} // namespace crf

