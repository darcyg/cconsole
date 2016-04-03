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

  ///std::size_t rmem;
  ///std::size_t vmem;
};
} // namespace proc
} // namespace crf

