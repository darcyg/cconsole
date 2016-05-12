/// sample.hpp
#pragma once
#include <sys/types.h>
#include <cstdlib>

namespace crf { namespace proc {
struct sample {
public:
  pid_t pid;
  double cpu_usage;
  std::size_t utime;
  std::size_t stime;

  std::size_t vm_rss;
  std::size_t vm_size;
  std::size_t vm_peak;
};
} // namespace proc
} // namespace crf

