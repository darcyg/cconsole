/// __cpu_time_impl.cpp
#include <cconsole/detail/__first_line_of.hpp>
#include <cconsole/detail/__startswith.hpp>
#include <cmonitor/detail/__cpu_time.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

std::size_t crf::proc::detail::cpu_total_of(std::string const& cpu_stat_line) {
  auto const first_space = cpu_stat_line.find( ' ' );
  auto num_b = cpu_stat_line.find_first_not_of( ' ', first_space );

  auto total = std::size_t{0u};
  while( num_b != std::string::npos )
  {
    auto const num_e = cpu_stat_line.find( ' ', num_b );
    total += boost::lexical_cast<std::size_t>(cpu_stat_line.substr( num_b, num_e-num_b  ));
    num_b = cpu_stat_line.find_first_not_of( ' ', num_e );
  }
  return total;
}


namespace {
static inline std::size_t jump_many(std::string const& str, std::size_t const start_posn, int const from, int const to) {
  auto posn = start_posn;
  for( auto i = from; i < to; ++i ) {
    posn = str.find_first_not_of( ' ', posn+1 );
    posn = str.find( ' ', posn+1 );
  }
  return posn;
}
} // namespace

crf::proc::detail::proc_stat crf::proc::detail::make_proc_stat(std::string const& proc_stat_line) {
  /// PID (command) S  
  /// man proc | less
  auto state_b = proc_stat_line.find( ')' ) + 1;
  state_b = proc_stat_line.find_first_not_of( ' ', state_b );
  auto const state = proc_stat_line[state_b]; 
  auto const state_e = state_b + 1;

  auto posn = ::jump_many( proc_stat_line, state_e, 3, 13 );
  auto const utime_b = proc_stat_line.find_first_not_of( ' ', posn );
  auto const utime_e = proc_stat_line.find( ' ', utime_b+1 );
  auto const utime   = boost::lexical_cast<std::size_t>(proc_stat_line.substr( utime_b, utime_e-utime_b ));

  auto const stime_b = proc_stat_line.find_first_not_of( ' ', utime_e );
  auto const stime_e = proc_stat_line.find( ' ', stime_b );
  auto const stime   = boost::lexical_cast<std::size_t>(proc_stat_line.substr( stime_b, stime_e-stime_b ));


  posn = ::jump_many( proc_stat_line, stime_e, 15, 19 );
  auto const num_thread_b = proc_stat_line.find_first_not_of( ' ', posn );
  auto const num_thread_e = proc_stat_line.find( ' ', num_thread_b+1 );
  auto const num_thread   = boost::lexical_cast<std::size_t>(proc_stat_line.substr( num_thread_b, num_thread_e-num_thread_b ));

  return { state, utime, stime, num_thread };
}

crf::proc::detail::cpu_time crf::proc::detail::make_cpu_time(std::string const& cpu_stat_line, std::string const& proc_stat_line) {
  auto const cpu_total = cpu_total_of(cpu_stat_line);
  auto const stat = make_proc_stat(proc_stat_line);
  return { stat.state, cpu_total, stat.utime, stat.stime };
}

crf::proc::detail::cpu_time crf::proc::make_cpu_time(std::string const& stat_path, std::string const& proc_stat_path) {
  auto stat_is = std::ifstream{stat_path}; 
  auto const stat_line = crf::first_line_of(stat_is, [] (auto const& s) noexcept { return crf::startswith( s, "cpu " ); });
  //if (stat_line.empty()) throw ..
  auto proc_stat_is = std::ifstream{proc_stat_path};
  auto const proc_stat_line = crf::first_line_of(proc_stat_is);
  //if (proc_stat_line.empty()) throw ..

  return crf::proc::detail::make_cpu_time( stat_line, proc_stat_line );
}

