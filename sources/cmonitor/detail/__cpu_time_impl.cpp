/// __cpu_time_impl.cpp
#include <cmonitor/detail/__cpu_time.hpp>
#include <boost/lexical_cast.hpp>

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

crf::proc::detail::proc_stat crf::proc::detail::make_proc_stat(std::string const& proc_stat_line) {
  // PID 
  auto const state_b = proc_stat_line.find( '(' ) + 1;
  auto const state_e = state_b + 1;
  auto const state = proc_stat_line[state_b]; 

  auto posn = state_e;
  for( auto i = 3; i < 14; ++i ) {
    posn = proc_stat_line.find_first_not_of( ' ', posn + 1 );
    posn = proc_stat_line.find( ' ', posn + 1 );
  }

  auto const utime_b = proc_stat_line.find_first_not_of( ' ', posn );
  auto const utime_e = proc_stat_line.find( ' ', utime_b + 1 );
  auto const utime   = boost::lexical_cast<std::size_t>(proc_stat_line.substr( utime_b, utime_e-utime_b ));

  auto const stime_b = proc_stat_line.find_first_not_of( ' ', utime_e );
  auto const stime_e = proc_stat_line.find( ' ', stime_b );
  auto const stime   = boost::lexical_cast<std::size_t>(proc_stat_line.substr( stime_b, stime_e-stime_b ));

  return { state, utime, stime };
}

crf::proc::detail::cpu_time crf::proc::detail::make_cpu_time(std::string const& cpu_stat_line, std::string const& proc_stat_line) {
  auto const cpu_total = cpu_total_of(cpu_stat_line);
  auto const stat = make_proc_stat(proc_stat_line);
  return { stat.state, cpu_total, stat.utime, stat.stime };
}
