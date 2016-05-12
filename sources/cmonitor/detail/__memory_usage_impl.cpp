/// __memory_usage_impl.cpp
#include <cmonitor/detail/__memory_usage.hpp>
#include <cconsole/detail/__startswith.hpp>
#include <boost/lexical_cast.hpp>


std::size_t crf::proc::detail::__integral_of(std::string const& line) {
  auto posn = line.find(':');
  if (posn == std::string::npos) return 0u;

  posn = line.find_first_not_of(" \t", posn+1);

  auto const int_b = posn;
  auto const int_e = line.find(' ', int_b);
  try {

    auto const count = (( int_e == std::string::npos ) ? std::string::npos : ( int_e - int_b ));
    return boost::lexical_cast<std::size_t>(line.substr(int_b, count));
  }
  catch( std::exception const& ) {
    return 0u;
  }
}

crf::proc::detail::memory_info crf::proc::detail::__memory_info_of(std::istream & stat_stream) noexcept {
#define __setattr(STRM, LN, ATTR, CND_STR) \
{\
  while (std::getline((STRM), (LN)) && ! crf::startswith((LN), (CND_STR)));\
  if ((STRM)) {\
    auto const value = detail::__integral_of((LN));\
    (ATTR) = value;\
  }\
}

  auto info = detail::memory_info{};
  std::string line;
  __setattr(stat_stream, line, info.vm_peak, "VmPeak:");
  __setattr(stat_stream, line, info.vm_size, "VmSize:");
  __setattr(stat_stream, line, info.vm_rss,  "VmRSS:");
  __setattr(stat_stream, line, info.threads, "Threads:");

#undef __setattr
  return info;
}

