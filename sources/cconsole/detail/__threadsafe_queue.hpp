/// __threadsafe_queue.hpp
#ifndef _CRF_THREADSAFE_QUEUE_HPP_
#define _CRF_THREADSAFE_QUEUE_HPP_
#include <cconsole/detail/__spinlock.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <queue>
#include <mutex>

namespace crf { namespace threadsafe {

template <typename T>
class queue : private boost::noncopyable {
public:
  using queue_t = std::queue<T>;
  using lock_t  = crf::spinlock;
  ///to comply to the standard :)
  using value_type = typename queue_t::value_type;

private:
  using guard_t = std::lock_guard<lock_t>;

  value_type pop_front() noexcept(noexcept(value_type{std::declval<value_type>()})) {
    auto element = _queue.front();
    _queue.pop();
    return element;
  } 

public:
  queue() = default;

  void push(T const& element) noexcept(noexcept(this->pop_front())) {
    guard_t g{_lock};
    _queue.push(element);
  }

  boost::optional<value_type> pop() noexcept(noexcept(this->pop_front())) {
    {
      guard_t g{_lock};
      if (not _queue.empty())
        return pop_front();
    }
    return boost::none;
  }

  bool empty() const noexcept {
    guard_t g{_lock};
    return _queue.empty();
  }

  std::size_t size() const noexcept {
    guard_t g{_lock};
    return _queue.size();
  }

private:
  mutable lock_t _lock;
  queue_t _queue;

};
} // namespace threadsafe
} // namespace crf

#endif // _CRF_THREADSAFE_QUEUE_HPP_

