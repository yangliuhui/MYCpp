#ifndef UTIL_TIMER_BASE_H
#define UTIL_TIMER_BASE_H
#include <chrono>
namespace mycpp {
namespace common {
class TimerBase {
public:
  TimerBase() = default;
  void Clear() { start_ = std::chrono::system_clock::time_point::min(); }
  bool IsStarted() const {
    return start_.time_since_epoch() != std::chrono::system_clock::duration(0);
  }
  void Start() { start_ = std::chrono::system_clock::now(); }
  unsigned long GetMs() {
    if (IsStarted()) {
      auto diff = std::chrono::system_clock::now() - start_;
      return static_cast<unsigned long>(
          std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
    }
    return 0;
  }

private:
  std::chrono::system_clock::time_point start_{
      std::chrono::system_clock::time_point::min()};
};
} // namespace common
} // namespace mycpp
#endif
