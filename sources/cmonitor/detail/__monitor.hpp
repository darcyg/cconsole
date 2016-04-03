/// __monitor.hpp
#pragma once
#include <cconsole/sample.hpp>
#include <boost/format.hpp>
#include <chrono>

namespace crf { namespace proc {
class monitor {
public:
  using sample_t = crf::proc::sample;

  /// to obtain cpu total from
  static auto constexpr _stat_file = "/proc/stat";
  static std::string const _stat_path;
  /// to obtain memory consumption data from
  static boost::format  _proc_status_file_format;
  /// to obtain runtime info from -- stime, utime, state
  static boost::format  _proc_stat_file_format;
  static auto constexpr _default_parse_interval = std::chrono::milliseconds{100};

  explicit monitor(pid_t const pid);
  sample_t sample(std::chrono::milliseconds const parse_interval = _default_parse_interval) const;

private:
  pid_t _pid;
  std::string _proc_stat_path;
  std::string _proc_status_path;
};


inline monitor make_monitor(pid_t const pid) noexcept __attribute__((always_inline)) __attribute__((flatten));
} // namespace proc
} // namespace crf


inline crf::proc::monitor crf::proc::make_monitor(pid_t const pid) noexcept {
  return monitor{pid};
}

