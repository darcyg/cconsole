/// spinlock.cpp
#include <cconsole/tools.hpp>
#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>

TEST(spinlock, stress) {
  auto const N = std::thread::hardware_concurrency();
  auto threads = std::vector<std::thread>{};
  auto actual_value = 0u;
  crf::spinlock lock;
  {
    std::atomic<bool> ready{false};
    auto const spin = [&ready] () noexcept { while (! ready.load(std::memory_order_relaxed)); };
    auto const volatile _ = crf::make_join_guard(threads);
    for( auto i = 0u; i < N; ++i )
      threads.emplace_back([&] () mutable noexcept 
      {
        spin();
        {
          std::lock_guard<crf::spinlock> lg{lock};
          actual_value += 1u; 
        }
      });

    ready.store(true, std::memory_order_relaxed);
  } // join-all

  auto const expected_value = N;
  EXPECT_EQ(actual_value, expected_value);
}

