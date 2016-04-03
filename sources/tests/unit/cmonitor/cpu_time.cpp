/// cpu_time.cpp
#include <cmonitor/detail/__cpu_time.hpp>
#include <gtest/gtest.h>

TEST(libccmonitor_cpu_time, cpu_total_of) {
  auto const expected_total = std::size_t{ 5u + 4u + 3u + 2u + 1u + 0u + 1u };
  for( auto const* line : { "cpu  5 4 3 2 1 0 1", "cpu  5 4 3 2 1 0 1 " }) 
  {
    auto const actual_total = crf::proc::detail::cpu_total_of( line );
    EXPECT_EQ( actual_total, expected_total );
  }
}

TEST(libccmonit_cpu_time, make_proc_stat) {
  /// (1) PID
  /// (2) (command)
  /// (3) State
  /// ..
  /// (14) utime
  /// (15) stime
  /// ..
  /// (20) num_thread
  auto const proc1_stat  = "1 (init) S 0 1 1 0 -1 4219136 73532 1104501 31 921 43 76 1562 675 20 0 1 0 3 34992128 871 18446744073709551615 1 1 0 0 0 0 0 4096 536962595 0 0 0 17 3 0 0 111 0 0 0 0 0 0 0 0 0 0";
  auto const proc77_stat = "77 (kswapd0) S 2 0 0 0 -1 10758208 0 0 0 0 0 0 0 0 20 0 1 0 58 0 0 18446744073709551615 0 0 0 0 0 0 0 2147483647 0 0 0 0 17 5 0 0 0 0 0 0 0 0 0 0 0 0 0";
  auto const proc78_stat = "78 (vmstat) S 2 0 0 0 -1 69247072 0 0 0 0 0 0 0 0 0 -20 1 0 58 0 0 18446744073709551615 0 0 0 0 0 0 0 2147483647 0 0 0 0 17 4 0 0 0 0 0 0 0 0 0 0 0 0 0";
}

