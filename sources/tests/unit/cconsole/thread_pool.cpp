/// thread_pool.cpp
#include <cconsole/thread_pool.hpp>
#include <gtest/gtest.h>
#include <chrono>

namespace {
static inline void expect_defaults(crf::thread_pool & pool, unsigned size = crf::thread_pool::_default_size) {
  EXPECT_EQ(pool.size(), size);
  EXPECT_FALSE(pool.running());
}
} // namespace

TEST(thread_pool, creation)
{
  {
    crf::thread_pool pool;
    ::expect_defaults(pool);
  }
  {
    auto const size = 21u;
    crf::thread_pool pool{size};
    ::expect_defaults(pool, size);
  }
}

TEST(thread_pool, stress)
{
  auto const N = std::thread::hardware_concurrency();
  crf::thread_pool pool{N};
  ::expect_defaults(pool, N);
  std::atomic<unsigned> actual_value{0u};
  
  auto futures = std::vector<std::future<void>>{};
  futures.reserve(N);
  for( auto i = 0u; i < N; ++i )
  {
    futures.push_back(pool.submit([&actual_value] () mutable noexcept 
    {
      std::this_thread::sleep_for(std::chrono::milliseconds{2});
      actual_value.fetch_add(1u, std::memory_order_relaxed);
    }));
    EXPECT_TRUE(pool.running());
  }

  for( auto&& f : futures ) f.get();
  pool.shutdown(); 

  EXPECT_FALSE(pool.running());
  auto const expected_value = N;
  EXPECT_EQ(actual_value.load(), expected_value);
}

