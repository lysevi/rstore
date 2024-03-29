#pragma once

#include <cstring>
#include <rstore/slice.h>
#include <rstore/utils/exception.h>
#include <type_traits>

namespace rstore {

template <typename T>
std::enable_if_t<std::is_pod_v<T>, T> slice_convert_to(const Slice &sl) {
  static_assert(std::is_pod_v<T>);
  ENSURE(sizeof(T) == sl.size);
  T result = {};
  std::memcpy(&result, sl.data, sl.size);
  return result;
}

template <typename T>
Slice slice_make_from(const T &v, const slice_size_t s) {
  static_assert(std::is_pod_v<T>);

  auto buf = new uint8_t[s];
  std::memcpy(buf, &v, s);

  return Slice(s, buf);
}

template <typename T>
std::enable_if_t<std::is_pod_v<T>, Slice> slice_make_from(const T &v) {
  constexpr auto s = sizeof(T);
  return slice_make_from(v, static_cast<slice_size_t>(s));
}
} // namespace rstore
