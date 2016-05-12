/// __memory_usage.hpp
#pragma once
#include <fstream>
#include <string>

namespace crf { namespace proc {
namespace detail {
struct memory_info {
  std::size_t vm_rss;
  std::size_t vm_size;
  std::size_t vm_peak;

  ///effectively: has meaningful values..
  explicit operator bool () const noexcept __attribute__((always_inline));
};


std::size_t __integral_of(std::string const& line) __attribute__((flatten));
memory_info __memory_info_of(std::istream & stat_stream) noexcept __attribute__((flatten));
} // namespace detail

inline detail::memory_info make_memory_info(std::string const& proc_status_path) __attribute__((always_inline)) __attribute__((flatten));
} // namespace proc
} // namespace crf

inline crf::proc::detail::memory_info::operator bool () const noexcept {
  return not(( vm_rss == 0u ) or ( vm_size == 0u ) or ( vm_peak == 0u ));
}

inline crf::proc::detail::memory_info crf::proc::make_memory_info(std::string const& proc_status_path) {
  ///TODONE: add check..
  std::ifstream stat_file{proc_status_path};
  return detail::__memory_info_of(stat_file); 
}

