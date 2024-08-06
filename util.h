#pragma once

#include <cassert>
#include <chrono>

class Timer final {
private:
  std::chrono::high_resolution_clock::time_point start_, end_;
  bool started_ = false;

public:
  Timer() = default;
  void start() noexcept {
    assert(!started_);
    started_ = true;
    start_ = std::chrono::high_resolution_clock::now();
  }
  void stop() noexcept {
    assert(started_);
    started_ = false;
    end_ = std::chrono::high_resolution_clock::now();
  }
  unsigned elapsed_ms() {
    assert(!started_);
    auto elps = end_ - start_;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elps);
    return msec.count();
  }
};