/// __thread_pool_impl.cpp
#include <cconsole/thread_pool.hpp>
#include <numeric>

using crf::thread_pool;

unsigned const thread_pool::_default_size = std::thread::hardware_concurrency();

thread_pool::thread_pool(unsigned size)
: _flags(size)
, _workers(size)
, _guard{_workers}
{
}

unsigned thread_pool::size() const noexcept {
  return _workers.size();
}

bool thread_pool::running(unsigned id) const noexcept {
  return static_cast<bool>(_flags[id]);
}

bool thread_pool::running() const noexcept {
  return std::accumulate(_flags.begin(), _flags.end(), true, [] (bool acc, auto const& flag) noexcept 
  {
    return( acc and flag );
  }); 
}

thread_pool::queue_t & thread_pool::queue() noexcept {
  return _queue;
}

void thread_pool::start() {
  if (running()) return;

  for( auto&& f : _flags ) f.set();
  for( std::size_t id = 0u, sz = _workers.size(); id < sz; ++id )
    _workers[id] = std::move(__worker(id, *this)); 
}

void thread_pool::shutdown() noexcept {
  for( auto&& f : _flags ) f.clear();
  ///wait all working threads..
  auto const _ = join_guard_t{_workers};
}


#define __chilled std::memory_order_relaxed
void thread_pool::__atomic_flag::clear() noexcept {
  _flag.store(false, __chilled);
}

void thread_pool::__atomic_flag::set() noexcept {
  _flag.store(true, __chilled);
}

thread_pool::__atomic_flag::operator bool () const noexcept {
  return _flag.load(__chilled);
}

