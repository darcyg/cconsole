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



