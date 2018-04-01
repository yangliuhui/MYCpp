#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
namespace mycpp {
namespace common {
template <typename T> class ThreadSafeQueue {
private:
  mutable std::mutex mut;
  std::queue<T> data_queue;
  std::condition_variable data_cond;

public:
  ThreadSafeQueue() {}
  ThreadSafeQueue(ThreadSafeQueue const &other) {
    std::lock_guard<std::mutex> lk(other.mut);
    data_queue = other.data_queue;
  }

  void Push(const T &new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond.notify_one();
  }
  void PushCap(std::size_t cap, const T &new_value) {
    std::lock_guard<std::mutex> lk(mut);
    while (data_queue.size() >= cap) {
      data_queue.pop();
    }
    data_queue.push(new_value);
    data_cond.notify_one();
  }
  void WaitPop(T &value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    value = data_queue.front();
    data_queue.pop();
  }

  std::shared_ptr<T> WaitPop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return res;
  }

  bool TryPop(T &value) {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty()) {
      return false;
    }
    value = data_queue.front();
    data_queue.pop();
    return true;
  }

  std::shared_ptr<T> TryPop() {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty()) {
      return false;
    }
    std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return res;
  }

  bool Empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};
} // namespace common
} // namespace mycpp
