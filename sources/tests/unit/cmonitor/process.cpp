/// process.cpp
#include <cmonitor/detail/__proc_guard.hpp>
#include <cmonitor/detail/__process.hpp>
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <unistd.h>
#include <chrono>
#include <thread>

#include <cstdio>

TEST(process, management)
{
  static auto _fail_message_format = boost::format{"[error] %1%"};

  auto const command = std::string{"./build/build/bin/simplex"};
  std::string cmdline;
  pid_t pid;
  try {

    pid = crf::proc::start(command);
    crf::proc::guard const volatile guard{pid};

    auto const this_pid = ::getpid();
    EXPECT_NE(pid, this_pid);

    ///we wait for exec() to load the new image in the child process..
    do
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds{210});
      cmdline = crf::proc::cmdline_of(pid);
    }
    while (cmdline.empty());
    EXPECT_FALSE(cmdline.empty());

    auto const this_cmdline  = crf::proc::cmdline_of(this_pid);
    EXPECT_FALSE(this_cmdline.empty());
    ASSERT_NE(cmdline, this_cmdline);
    EXPECT_TRUE(crf::proc::is_alive(pid, cmdline));

    // killing the process happens here..
  }
  catch( std::exception const& ex ) {
    auto const error_message = ( _fail_message_format % ex.what() ).str();
    ASSERT_STREQ(error_message.c_str(), "");
  }

  ///we wait for the proc to be killed / cleaned up properly..
  std::this_thread::sleep_for(std::chrono::milliseconds{130}); 
  EXPECT_FALSE(crf::proc::is_alive(pid, cmdline));
}

