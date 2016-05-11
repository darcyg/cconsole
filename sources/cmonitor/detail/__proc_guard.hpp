/// __proc_guard.hpp
#pragma once
#include <cmonitor/detail/__process.hpp>
#include <boost/noncopyable.hpp>

namespace crf { namespace proc {
struct guard : private boost::noncopyable {
public:
  explicit guard(pid_t pid) noexcept
  : _pid{pid}
  {
  }

  ~guard() {
   if( _pid > 1 )
     try{ crf::proc::kill(_pid); } catch( ... ) { /*we do not care..*/ }
  }

private:
  pid_t _pid;

};
} // namespace proc
} // namespace crf

