/// __spinlock.hpp
#ifndef _CRF_SPINLOCK_HPP_
#define _CRF_SPINLOCK_HPP_
#include <cconsole/convenience.hpp>
#include <atomic>

#include <cstdio>

namespace crf {
struct spinlock {
public:
  constexpr CRF_ALWAYS_INLINE CRF_FLATTEN spinlock() noexcept;
  void CRF_ALWAYS_INLINE CRF_FLATTEN lock() noexcept;
  void CRF_ALWAYS_INLINE CRF_FLATTEN unlock() noexcept;

private:
  ///guaranteed to be lock-free
  std::atomic_flag _locked;

};
} // namespace crf


constexpr crf::spinlock::spinlock() noexcept
: _locked{ATOMIC_FLAG_INIT}
{
}

inline
void crf::spinlock::lock() noexcept {
  /*
  while the previous value obtained is true we know we're not the first
  to set the flag and attempt to enter the critical region thus
  we need to wait for them to finish..
  */
  while (_locked.test_and_set(std::memory_order_acquire)) {/* empty */}
}

inline
void crf::spinlock::unlock() noexcept {
  /*
  leaving the critical region: we clear the flag allowing
  someone else to enter the critical region..
  */
  _locked.clear(std::memory_order_release);
}

#endif // _CRF_SPINLOCK_HPP_

