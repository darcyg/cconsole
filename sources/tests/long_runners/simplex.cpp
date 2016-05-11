/// simplex.cpp
#include <cstdio>
#include <thread>
#include <atomic>
#include <chrono>
#include <random>
#include <ctime>
#include <cmath>
#include <map>

namespace {
void do_some_heavy_calculation(long iterations = 130l) noexcept {
  std::srand(std::time(0));  
  while (--iterations > 0l)
  {
    std::atomic<int> random_number{std::rand()};
    while (( random_number.exchange(std::sqrt(random_number.load())) ) > 1000);
  }
}
} // namespace 

int main(int argc, char * argv[]) {
  //auto i = 130;
  while (true)
  {
     auto timeout = 1300;
     while (--timeout > 0)
       ::do_some_heavy_calculation();

     std::this_thread::sleep_for(std::chrono::milliseconds{7});   
  }
}

