/// monitor.cpp
#include <cmonitor/detail/__proc_guard.hpp>
#include <cmonitor/detail/__process.hpp>
#include <cmonitor/detail/__monitor.hpp>
#include <gtest/gtest.h>

TEST(monitor, ownership) {
  ///test owning monitor
  auto const command = "./build/build/bin/simplex";
  {
    std::string cmdline;
    pid_t pid;
    {
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
    EXPECT_FALSE(crf::proc::is_alive(pid, cmdline));
  }

  ///test NOT owning monitor
  std::string cmdline;
  pid_t pid;
  {
    pid = crf::proc::start(command);
    crf::proc::guard const volatile guard{pid};
    {
      crf::proc::monitor m{pid};
      EXPECT_TRUE(static_cast<bool>(m));
      EXPECT_FALSE(m.is_owner());

      cmdline = m.cmdline();
      EXPECT_GT(pid, 1);
      EXPECT_EQ(m.is_process_alive(), crf::proc::is_alive(pid, cmdline));
    }
    EXPECT_TRUE(crf::proc::is_alive(pid, cmdline));
  }
  EXPECT_FALSE(crf::proc::is_alive(pid, cmdline));
}


