/// sample.hpp
#pragma once
#include <sys/types.hpp>

namespace crf { namespace proc_info {
struct sample {
public:
  pid_t pid;
  double cpu_usage;
  std::size_t utime;
  std::size_t stime;

  ///std::size_t rmem;
  ///std::size_t vmem;
};
} // namespace proc_info
} // namespace crf

