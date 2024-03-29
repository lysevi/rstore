#pragma once

#include <ctime>
#include <thread>

#include <rstore/utils/exception.h>

#define UNUSED(x) (void)(x)

namespace rstore::utils {

inline void sleep_mls(long long a) {
  std::this_thread::sleep_for(std::chrono::milliseconds(a));
}

struct ElapsedTime {
  ElapsedTime() noexcept { start_time = std::clock(); }
  [[nodiscard]] double elapsed() noexcept {
    return ((double)std::clock() - start_time) / CLOCKS_PER_SEC;
  }
  std::clock_t start_time;
};

template <class T>
void insert_to_array(T *array, size_t sz, size_t insert_pos, const T &value) {
  for (auto i = sz - 1; i > insert_pos; i--) {
    std::swap(array[i], array[i - 1]);
  }
  array[insert_pos] = value;
}

} // namespace rstore::utils
