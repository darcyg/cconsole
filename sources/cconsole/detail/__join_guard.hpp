/// __join_guard.hpp
#ifndef _CRF_JOIN_GUARD_HPP_
#define _CRF_JOIN_GUARD_HPP_
#include <boost/noncopyable.hpp>
#include <type_traits>
#include <utility>
#include <vector>

namespace crf {
namespace detail {
template <typename T>
class __is_joinable {
private:
  template <typename U> static auto test(U * u) noexcept -> decltype(u->joinable(), u->join(), std::true_type{});
  template <typename U> static auto test(...) noexcept -> std::false_type;

public:
  static auto constexpr value = decltype(test<T>(0))::value;

};
} // namespace detail

template <typename Joinable>
class join_guard : private boost::noncopyable {
private:
  template <typename J> static void __join(J && j) {
    if (j.joinable())
      j.join();
  }

  template <typename Joinables> static void __join_all(Joinables * joinables) {
    if (! joinables) return;
    for( auto&& j : *joinables )
      __join(j);
  }

public:
  using joinable_t = typename std::remove_reference<Joinable>::type;
  
  explicit join_guard(std::vector<joinable_t> & joinables) noexcept
  : _joinables{&joinables}
  {
  }

  join_guard(join_guard && other) noexcept : _joinables{nullptr} {
    std::swap(_joinables, other._joinables);
  }
  join_guard& operator= (join_guard &&) = delete;

  ~join_guard() {
    __join_all(_joinables);
  }

private:
  std::vector<joinable_t> * _joinables;  

};


template <typename Joinable>
inline crf::join_guard<Joinable> make_join_guard(std::vector<Joinable> & joinables) noexcept {
  static_assert(crf::detail::__is_joinable<Joinable>::value, "Parameter must be a collection of joinables -- see std::thread");
  return std::move(crf::join_guard<Joinable>(joinables));
}
} // namespace crf

#endif // _CRF_JOIN_GUARD_HPP_

