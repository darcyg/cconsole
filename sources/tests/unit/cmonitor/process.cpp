/// process.cpp
#include <cmonitor/detail/__proc_guard.hpp>
#include <cmonitor/detail/__process.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <unistd.h>
#include <chrono>
#include <thread>

#include <cstdio>

/*
TEST(process, management)
{
  static auto _fail_message_format = boost::format{"[error] %1%"};

  auto const command = std::string{"./build/build/bin/simplex"};
  pid_t pid;
  try {

    pid = crf::proc::start(command);
  }
  catch( std::exception const& e ) {
    std::printf("[warning] unable to start %s due to getting: %s -- skipping..\n", command.c_str(), e.what());
    ASSERT_STRNE("", "");
  }

  std::string cmdline;
  try {

    crf::proc::guard const volatile guard{pid};
    auto const ppid = ::getpid();
    EXPECT_NE(pid, ppid);

    ///we wait for exec() to load the new image in the child process..
    auto const parent_cmdline  = crf::proc::cmdline_of(ppid);
    EXPECT_FALSE(parent_cmdline.empty());
    do
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds{210});
      cmdline = crf::proc::cmdline_of(pid);
    }
    while (cmdline.empty() or ( cmdline == parent_cmdline ));

    EXPECT_FALSE(cmdline.empty());
    ASSERT_NE(cmdline, parent_cmdline);
    EXPECT_TRUE(crf::proc::is_alive(pid, cmdline));

    // killing the process happens here..
  }
  catch( std::exception const& ex ) {
    auto const error_message = ( _fail_message_format % ex.what() ).str();
    ASSERT_STREQ(error_message.c_str(), "");
  }

  EXPECT_FALSE(crf::proc::is_alive(pid, cmdline));
}
*/

