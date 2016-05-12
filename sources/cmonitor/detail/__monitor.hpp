/// __monitor.hpp
#pragma once
#include <cmonitor/detail/__proc_guard.hpp>
#include <boost/noncopyable.hpp>
#include <cconsole/sample.hpp>
#include <boost/optional.hpp>
#include <boost/format.hpp>
#include <chrono>
#include <memory>


namespace crf { namespace proc {
class monitor : private boost::noncopyable {
public:
  using sample_t = crf::proc::sample;

  static auto constexpr _default_parse_interval = std::chrono::milliseconds{42};

  explicit monitor(pid_t const pid);
  explicit monitor(std::string const& cmdline);
  ~monitor();

  sample_t sample(std::chrono::milliseconds const parse_interval = _default_parse_interval) const;

  pid_t pid() const noexcept;
  std::string const& cmdline() const noexcept;
  bool is_owner() const noexcept;
  bool is_process_alive() const noexcept;

  ///effectively is_not_detached() == pid > 0
  explicit operator bool () const noexcept;

private:
  using optional_guard_t = boost::optional<crf::proc::guard>;
  struct __descriptor;

  monitor(pid_t const pid, std::string const& cmdline, bool const is_owner);
  monitor(pid_t const pid, bool const is_owner);

  __descriptor * _dsc_ptr;
  bool _owner;

  optional_guard_t _guard;
};
} // namespace proc
} // namespace crf

