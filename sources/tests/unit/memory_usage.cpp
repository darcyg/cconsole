/// memory_usage.cpp
#include <cmonitor/detail/__memory_usage.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(memory_usage, __integral_of) {
  try {

    EXPECT_EQ(12, crf::proc::detail::__integral_of("VmSize:	12 kB"));
    EXPECT_EQ(12, crf::proc::detail::__integral_of("VmRSS:	12 kB"));
    EXPECT_EQ(1,  crf::proc::detail::__integral_of("Threads:		1"));
    EXPECT_EQ(0u, crf::proc::detail::__integral_of("VmPeak:	abcd kB"));
    EXPECT_EQ(0u, crf::proc::detail::__integral_of("Threads:	abcd"));

    EXPECT_EQ(12, crf::proc::detail::__integral_of("VmSize:       12 kB"));
    EXPECT_EQ(12, crf::proc::detail::__integral_of("VmRSS:        12 kB"));
    EXPECT_EQ(1,  crf::proc::detail::__integral_of("Threads:             1"));
    EXPECT_EQ(0u, crf::proc::detail::__integral_of("VmPeak:       abcd kB"));
    EXPECT_EQ(0u, crf::proc::detail::__integral_of("Threads:      abcd"));
  }
  catch (...) {
    ASSERT_STREQ("[error] unexpected exception", "");
  }
}

TEST(memory_usage, __memory_info_of) {
  {
    auto const status =
       "VmPeak:		12 kb\n"
       "VmSize:		12 kb\n"
       "VmLck:		0 kb\n"
       "VmRSS:		12 kb\n"
       "VmSwap:		0 kb\n"
       "Threads:		1\n";

    std::istringstream iss{status};
    auto const mi = crf::proc::detail::__memory_info_of(iss);
    EXPECT_EQ(mi.vm_peak, 12);
    EXPECT_EQ(mi.vm_size, 12);
    EXPECT_EQ(mi.vm_rss,  12);
    EXPECT_EQ(mi.threads, 1);
    EXPECT_TRUE(static_cast<bool>(mi));
  }
  {
    auto const status =
       "VmPeak:		12 kb\n"
       "VmLck:		0 kb\n"
       "VmRSS:		12 kb\n"
       "VmSwap:		0 kb\n"
       "Threads:		1\n";

    std::istringstream iss{status};
    auto const mi = crf::proc::detail::__memory_info_of(iss);
    EXPECT_EQ(mi.vm_peak, 12);
    EXPECT_EQ(mi.vm_size, 0);
    EXPECT_EQ(mi.vm_rss,  0);
    EXPECT_EQ(mi.threads, 0);
    EXPECT_FALSE(static_cast<bool>(mi));
  }
  {
    auto const status = "";
    std::istringstream iss{status};
    auto const mi = crf::proc::detail::__memory_info_of(iss);
    EXPECT_EQ(mi.vm_peak, 0);
    EXPECT_EQ(mi.vm_size, 0);
    EXPECT_EQ(mi.vm_rss,  0);
    EXPECT_EQ(mi.threads, 0);
    EXPECT_FALSE(static_cast<bool>(mi));
  }
  {
    auto const status =
       "VmPeak:		12 kB\n"
       "VmSize:		12 kB\n"
       "VmLck:		0 kB\n"
       "VmRSS:		12 kB\n"
       "VmSwap:		0 kB\n";

    std::istringstream iss{status};
    auto const mi = crf::proc::detail::__memory_info_of(iss);
    EXPECT_EQ(mi.vm_peak, 12);
    EXPECT_EQ(mi.vm_size, 12);
    EXPECT_EQ(mi.vm_rss,  12);
    EXPECT_EQ(mi.threads, 0);
    EXPECT_FALSE(static_cast<bool>(mi));
  }
  {
    auto const status =
       "VmPeak:		12 kb\n"
       "VmSize:		12 kb\n"
       "VmRSS:		12 kb\n"
       "Threads:		1\n";

    std::istringstream iss{status};
    auto const mi = crf::proc::detail::__memory_info_of(iss);
    EXPECT_EQ(mi.vm_peak, 12);
    EXPECT_EQ(mi.vm_size, 12);
    EXPECT_EQ(mi.vm_rss,  12);
    EXPECT_EQ(mi.threads, 1);
    EXPECT_TRUE(static_cast<bool>(mi));
  }
}

