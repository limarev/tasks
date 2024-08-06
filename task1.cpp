#include <cassert>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

#include "util.h"

struct Location {
  double lat; // широта
  double lng; // долгота

  bool operator==(const Location &other) const noexcept { return lat == other.lat && lng == other.lng; };
};

std::ostream &operator<<(std::ostream &os, Location c) { return os << c.lat << ' ' << c.lng; }

template <typename T> T Limiter();
template <> Location Limiter<Location>() {
  return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
}

struct Config {
  int tasksNumber = 100;
  std::pair<int, int> ptime{200, 500}; // ms
  std::pair<int, int> ctime{100, 600}; // ms
};

template <typename T> class TSQueue {
private:
  std::queue<T> q_;
  mutable std::mutex mut_;
  std::condition_variable cond_;

public:
  void push(T value) {
    std::lock_guard<std::mutex> lk{mut_};
    q_.push(std::move(value));
    cond_.notify_one();
  }

  bool wait_and_pop(T &value) {
    std::unique_lock<std::mutex> lk{mut_};
    cond_.wait(lk, [this] { return !q_.empty(); });
    auto &&task = q_.front();
    if (task == Limiter<T>()) {
      q_.pop();
      return false;
    }

    std::swap(value, task);
    q_.pop();

    return true;
  }
};

template <typename T> T random(T left, T right) {
  assert(left < right);
  std::random_device rd;
  std::default_random_engine e1(rd());
  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> dist(left, right);
    return dist(e1);
  }
  if constexpr (std::is_floating_point_v<T>) {
    std::uniform_real_distribution<T> dist(left, right);
    return dist(e1);
  }
}

Location generate() {
  constexpr double pi = 3.141592653589793238462643383279502;
  return {
      random(-pi / 2, pi / 2),
      random(-pi, pi),
  };
}

std::chrono::milliseconds to_ms(int t) { return std::chrono::duration<int, std::milli>(t); }

void produce(TSQueue<Location> &q, Config cfg) {
  for (int i = 0; i < cfg.tasksNumber; ++i) {
    std::this_thread::sleep_for(to_ms(random(cfg.ptime.first, cfg.ptime.second)));
    q.push(generate());
  }
  q.push(Limiter<Location>());
}

void consume(TSQueue<Location> &q, Config cfg) {
  Location value;
  while (q.wait_and_pop(value)) {
    Timer t;
    t.start();

    std::this_thread::sleep_for(to_ms(random(cfg.ctime.first, cfg.ctime.second)));

    t.stop();

    std::cout << value << ' ' << t.elapsed_ms() << '\n';
  }
}

int main() {
  try {
    TSQueue<Location> queue;

    Config config;

    std::thread producer(produce, std::ref(queue), config);
    std::thread consumer(consume, std::ref(queue), config);

    Timer t;
    t.start();

    producer.join();
    consumer.join();

    t.stop();
    std::cout << t.elapsed_ms() << '\n';

  } catch (const std::exception &ex) {
    std::cerr << ex.what();
    return -1;
  } catch (...) {
    std::cerr << "Unknown error\n";
    return -1;
  }
  return 0;
}