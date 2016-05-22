/// monitor.cpp
#include <cmonitor/detail/__proc_guard.hpp>
#include <cmonitor/detail/__process.hpp>
#include <cmonitor/detail/__monitor.hpp>
#include <gtest/gtest.h>
#include <thread>

#include <cstdio>


TEST(monitor, ownership) {
  ///test owning monitor
  auto const command = "./build/build/bin/simplex";
  {
    std::string cmdline;
    pid_t pid;
    try {

      crf::proc::monitor monitor{command};
      EXPECT_TRUE(static_cast<bool>(monitor));
      EXPECT_TRUE(monitor.is_owner());

      pid = monitor.pid();
      cmdline = monitor.cmdline();
      EXPECT_FALSE(cmdline.empty());
      EXPECT_GT(pid, 1);
      EXPECT_TRUE(crf::proc::is_alive(pid, cmdline));
      EXPECT_EQ(monitor.is_process_alive(), crf::proc::is_alive(pid, cmdline));

    }
    catch( std::exception const& e ) {
      std::printf("[warning] unable to start %s due to getting: %s -- skipping..\n", command, e.what());
      return;
    }
    EXPECT_FALSE(crf::proc::is_alive(pid, cmdline));
  }

  ///test NOT owning monitor
  std::string cmdline;
  pid_t pid;
  {
    pid = crf::proc::start(command);
    crf::proc::guard const volatile guard{pid};
    try {

      crf::proc::monitor m{pid};
      EXPECT_TRUE(static_cast<bool>(m));
      EXPECT_FALSE(m.is_owner());

      cmdline = m.cmdline();
      EXPECT_GT(pid, 1);
      EXPECT_EQ(m.is_process_alive(), crf::proc::is_alive(pid, cmdline));

    }
    catch( std::exception const& e ) {
      std::printf("[warning] unable to monitor %d due to getting: %s -- skipping..\n", pid, e.what());
      return;
    }
    EXPECT_TRUE(crf::proc::is_alive(pid, cmdline));
  }
  EXPECT_FALSE(crf::proc::is_alive(pid, cmdline));
}


/*
namespace debug {
static inline void __log(crf::proc::monitor::sample_t const& sample) noexcept {
  std::printf("\n");
  std::printf("pid:    %d\n",  sample.pid);
  std::printf("usage:  %f\n",  sample.cpu_usage);
  std::printf("utime:  %zu\n", sample.utime);
  std::printf("stime:  %zu\n", sample.stime);
  std::printf("VmRSS:  %zu\n", sample.vm_rss);
  std::printf("VmSize: %zu\n", sample.vm_size);
  std::printf("VmPeak: %zu\n", sample.vm_peak);
}
} // namespace debug
*/

TEST(monitor, sampling) {
  auto const command  = "./build/build/bin/simplex";
  auto const interval = std::chrono::milliseconds{21};
  try {

    crf::proc::monitor m{command};
    EXPECT_TRUE(m.is_process_alive());
    auto attempt = 130;
    auto sample = crf::proc::monitor::sample_t{};
    do {
  
      auto const prev = sample;
      sample = m.sample();
      EXPECT_NE(sample.utime, 0);
      EXPECT_TRUE(sample.utime >= prev.utime);
      EXPECT_TRUE(sample.stime >= prev.stime);

      /*debug::__log(sample);*/
      std::this_thread::sleep_for(interval);
    }
    while( --attempt > 0 );

  }
  catch( std::exception const& e ) {
    std::printf("[warning] unable to start %s due to getting: %s -- skipping..\n", command, e.what());
    return;
  }
}

