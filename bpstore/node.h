#pragma once
#include <bpstore/exports.h>
#include <bpstore/utils/exception.h>

#include <cstdint>

namespace bpstore {
using slice_size_t = uint32_t;
struct slice_t {
  slice_size_t size;
  uint8_t *data;

  slice_t(slice_size_t sz, uint8_t *val)
      : size(sz)
      , data(val) {}

  slice_t()
      : size(0)
      , data(nullptr) {}

  ~slice_t() { delete data; }

  // <  0 if this <  o,
  // == 0 if this == o,
  // >  0 if this >  o
  EXPORT int compare(const slice_t &o) const;
};

using storage_ptr_t = uint64_t;

struct node_t {
  bool is_leaf;
  uint32_t capacity;
  uint32_t size;

  slice_t *keys;
  slice_t *values;
  storage_ptr_t *children;

  EXPORT node_t(bool is_leaf_, uint32_t cap);
  EXPORT ~node_t();

  EXPORT bool insert(slice_t &k, slice_t &v);
};

} // namespace bpstore