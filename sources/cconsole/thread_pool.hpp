/// thread_pool.hpp
#ifndef _CRF_THREAD_POOL_HPP_
#define _CRF_THREAD_POOL_HPP_
#include <boost/lockfree/queue.hpp>
#include <functional>
#include <vector>
#include <thread>
#include <future>

#include <atomic>

namespace crf {
///-----------------
class thread_pool {
///-----------------
public:
  using task_t = std::function<void(void)>;
  static auto const _default_size; //= std::thread::hardware_concurrency();

  ///lazy init..
  explicit thread_pool(unsigned size = _default_size) noexcept;

  template <typename F> std::future<decltype(std::declval<F>() ())> submit(F && f);
  bool running() const noexcept;
  void shutdown() noexcept;

private:
  ///control_block:
  ///* workers    : std::vector<__worker>
  ///* guard      : crf::join_guard<...> 
  ///* run_manual : std::vector<__atomic_flag>
  ///* size       : unsigned
  /// > running()
  /// > running(unsigned)
  /// > shutdown()
  struct __control_block;
  //struct __atomic_flag ///< alignas(CACHE_LINE)

  ///worker:
  ///* id    : unsigned
  ///* cb    : __control_block &  
  ///* queue : boost::lockfree::queue<work_t> &
  //struct __worker ///< possibly a lambda-expression would suffice :)

  ///use efficient-pimpl (??)
  __control_block _cb;
  boost::lockfree::queue<task_t> _queue;
};
} // namespace crf

#endif // _CRF_THREAD_POOL_HPP_

