/// __monitor.hpp
#pragma once
#include <cconsole/sample.hpp>
#include <boost/format.hpp>

namespace crf { namespace proc {
class monitor {
public:
  using sample_t = crf::proc::sample;

  static boost::format _stat_file_format;

  explicit monitor(pid_t const pid) noexcept;
  sample_t sample() const;

private:
  pid_t _pid;
};


inline monitor make_monitor(pid_t const pid) noexcept __attribute__((always_inline)) __attribute__((flatten));
} // namespace proc
} // namespace crf

inline crf::proc::monitor crf::proc::make_monitor(pid_t const pid) noexcept {
  return monitor{pid};
}

