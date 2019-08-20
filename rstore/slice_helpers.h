#pragma once

#include <cstring>
#include <rstore/slice.h>
#include <rstore/utils/exception.h>
#include <type_traits>

namespace rstore {

template <typename T>
T slice_convert_to(const Slice &sl) {
  static_assert(std::is_pod_v<T>);
  ENSURE(sizeof(T) == sl.size);
  T result;
  std::memcpy(&result, sl.data, sl.size);
  return result;
}

template <typename T>
static Slice slice_make_from(const T *v, const slice_size_t s) {
  static_assert(std::is_pod_v<T>);

  return Slice{s, reinterpret_cast<const char *>(v)};
}

template <typename T>
static std::enable_if_t<std::is_pod_v<T>, Slice> slice_make_from(const T *v) {
  auto s = sizeof(T);
  return slice_make_from(v, static_cast<slice_size_t>(s));
}
} // namespace rstore
