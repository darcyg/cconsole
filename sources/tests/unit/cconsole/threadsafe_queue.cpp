/// threadsafe_queue.cpp
#include <cconsole/tools.hpp>
#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>

TEST(threadsafe_queue, stress) {
  auto const N = std::thread::hardware_concurrency();
  auto threads = std::vector<std::thread>{};
  threads.reserve(N);
  crf::threadsafe::queue<int> q;
  {
    std::atomic<bool> ready{false};
    auto const spin = [&ready] () noexcept { while (! ready.load(std::memory_order_relaxed)); };
    auto const volatile _ = crf::make_join_guard(threads);
    for( auto i = 0u; i < N; ++i )
      threads.emplace_back([&, i] () mutable noexcept 
      {
        spin();
        q.push(i);
      });

    ready.store(true, std::memory_order_relaxed);
  } // join-all

  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.size(), N);

  threads = std::vector<std::thread>();
  threads.reserve(N);
  std::vector<std::atomic<bool>> found(N);
  for( auto&& v : found ) v.store(true, std::memory_order_relaxed);
  {
    std::atomic<bool> ready{false};
    auto const spin = [&ready] () noexcept { while (! ready.load(std::memory_order_relaxed)); };
    auto const volatile _ = crf::make_join_guard(threads);
    for( auto i = 0u; i < N; ++i )
      threads.emplace_back([&] () mutable noexcept 
      {
        spin();
        auto const v = q.pop();
        if (not v) return;

        found[v.value()].store(true); 
      });

    ready.store(true, std::memory_order_relaxed);
  }

  for( auto&& v : found ) EXPECT_TRUE(v.load());
}

