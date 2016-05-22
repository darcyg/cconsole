/// thread_pool.hpp
#ifndef _CRF_THREAD_POOL_HPP_
#define _CRF_THREAD_POOL_HPP_
#include <cconsole/detail/__threadsafe_queue.hpp>
#include <cconsole/detail/__join_guard.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include <vector>
#include <thread>
#include <future>
#include <memory>

#include <atomic>


namespace crf {
///---------------------------------------------
class thread_pool : private boost::noncopyable {
///---------------------------------------------
public:
  using task_t  = std::function<void(void)>;
  using queue_t = crf::threadsafe::queue<task_t>; 
  static unsigned const _default_size; //= std::thread::hardware_concurrency();

  ///lazy init..
  explicit thread_pool(unsigned size = _default_size);
  ~thread_pool() = default;

  template <typename F> std::future<decltype(std::declval<F>() ())> submit(F && f);
  bool running() const noexcept;
  ///blocks inherently
  void shutdown() noexcept;
  unsigned size() const noexcept;

private:
  ///worker:
  ///* id    : unsigned
  ///* cb    : __control_block &  
  ///* queue : boost::lockfree::queue<work_t> &
  struct __worker; ///< possibly a lambda-expression would suffice :)
  using join_guard_t = crf::join_guard<__worker>;

  void start();
  bool running(unsigned id) const noexcept;
  queue_t & queue() noexcept;

  ///control_block:
  ///* workers    : std::vector<__worker>
  ///* guard      : crf::join_guard<...> 
  ///* run_manual : std::vector<__atomic_flag>
  ///* size       : unsigned
  /// > running()
  /// > running(unsigned)
  /// > shutdown()
  //struct __control_block;
  struct __atomic_flag; ///< alignas(CACHE_LINE)

  template <typename ReturnValue, typename Derived> struct __task_base;
  template <typename ReturnValue> struct __task;


  //__control_block _cb;
  std::vector<__atomic_flag> _flags;
  std::vector<__worker> _workers;
  join_guard_t _guard;

  queue_t _queue;
};

//---------------------------------------------
struct alignas(64) thread_pool::__atomic_flag {
//---------------------------------------------
public:
  constexpr __atomic_flag() noexcept
  : _flag{false}
  {
  }

  void clear() noexcept;
  void set() noexcept;

  explicit operator bool () const noexcept;

private:
  std::atomic<bool> _flag;

};


template <typename ReturnValue, typename Derived>
//-------------------------------
struct thread_pool::__task_base {
//-------------------------------
public:
  using promise_t = std::promise<ReturnValue>;
  using promise_pointer = std::shared_ptr<promise_t>;
  using function_t = std::function<ReturnValue(void)>;

  template <typename F>
  __task_base(promise_pointer promise_ptr, F && f)
  : _promise_ptr{std::move(promise_ptr)}
  , _function{std::forward<F>(f)}
  {
  }

  __task_base(__task_base && other) noexcept
  : _promise_ptr{std::move(other._promise_ptr)}
  , _function{std::move(other._function)}
  {
  }

  __task_base& operator= (__task_base && other) noexcept {
    if( this != &other ) {
      _promise_ptr = std::move(other._promise_ptr);
      _function = std::move(other._function);
    }
    return *this;
  }

  ReturnValue operator() (void) noexcept
  try {

    static_cast<Derived*>(this)->set_value(*_promise_ptr, _function);
  }
  catch (...) {
    _promise_ptr->set_exception(std::current_exception());
  }

  ~__task_base() = default;

private:
  promise_pointer _promise_ptr;
  function_t _function;

};


template <typename ReturnValue>
//----------------------------------------------------------------------------------------------
struct thread_pool::__task : public thread_pool::__task_base<ReturnValue, __task<ReturnValue>> {
//----------------------------------------------------------------------------------------------
private:
  using base_t = thread_pool::__task_base<ReturnValue, __task<ReturnValue>>;

public:
  using base_t::base_t;

private:
  void set_value(typename base_t::promise_t & promise, typename base_t::function_t & function) {
    auto r = function();
    promise.set_value(r);
  }

  friend base_t;
};


template <>
//--------------------------------------------------------------------------------------
struct thread_pool::__task<void> : public thread_pool::__task_base<void, __task<void>> {
//--------------------------------------------------------------------------------------
private:
  using base_t = thread_pool::__task_base<void, __task<void>>;

public:
  using base_t::base_t;

private:
  void set_value(typename base_t::promise_t & promise, typename base_t::function_t & function) {
    function();
    promise.set_value();
  }

  friend base_t;
};


//-------------------------------------------------
struct thread_pool::__worker : public std::thread {
//-------------------------------------------------
public:
#define __yield() \
{\
  std::this_thread::yield();\
  continue;\
}\

  __worker() noexcept : std::thread{} {}

  __worker(unsigned id, thread_pool & owner)
  : std::thread{[id, &owner] (void) mutable noexcept 
  {
   while (owner.running(id))
   {
    //optional
    auto task = owner.queue().pop(); 
    if (not task) __yield();

    task.value() ();
   } 
  }}
  {
  }

#undef __yield
};
} // namespace crf


template <typename F>
std::future<decltype(std::declval<F>() ())> crf::thread_pool::submit(F && f) {
  using return_t  = decltype(std::declval<F>() ());
  using promise_t = std::promise<return_t>;

  auto promise_ptr = std::make_shared<promise_t>();
  auto future = promise_ptr->get_future();
  /*
  _queue.push([promise{std::move(promise), f{std::forward<F>(f)}] () mutable noexcept 
  {
    try {

      auto ret = f();
      promise.set_value(ret);
    }
    catch (...) {
      promise.set_exception(std::current_exception());
    }
  });
  */
  _queue.push([promise_ptr{std::move(promise_ptr)}, f{std::forward<F>(f)}] () mutable noexcept
  {
     auto task = __task<return_t>{std::move(promise_ptr), std::forward<F>(f)};
     task(); 
  });

  start();
  return std::move(future);
}

#endif // _CRF_THREAD_POOL_HPP_

