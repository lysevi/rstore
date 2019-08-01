#include <bpstore/node.h>
#include <bpstore/node_helpers.h>

#include "helpers.h"
#include <algorithm>
#include <utility>

#include <catch.hpp>

TEMPLATE_TEST_CASE("storage.slice_t", "[store]", size_t, uint32_t, uint64_t) {
  using namespace bpstore;
  for (TestType i = 1; i < 10; ++i) {
    auto slc = slice_make_from(i, sizeof(i));
    EXPECT_EQ(static_cast<TestType>(slc.size), sizeof(i));
    EXPECT_TRUE(
        std::any_of(slc.data, slc.data + slc.size, [](auto v) { return v != 0; }));

    auto converted = slice_convert_to<TestType>(slc);
    EXPECT_EQ(i, converted);

    auto slc2 = slice_make_from(i + 1);
    EXPECT_EQ(slc2.size, slc.size);
    EXPECT_EQ(slc.compare(slc2), -1);
    EXPECT_EQ(slc.compare(slc), 0);
    EXPECT_EQ(slc2.compare(slc), 1);
  }
}

TEMPLATE_TEST_CASE("storage.node_t", "[store]", (std::pair<size_t, uint8_t>)) {
  using namespace bpstore;
  using test_key_type = typename TestType::first_type;
  using test_val_type = typename TestType::second_type;

  uint32_t node_cap = 10;
  node_t n(true, node_cap);

  for (test_key_type k = 0; k < static_cast<test_key_type>(node_cap); ++k) {
    for (test_val_type v = 0; v < static_cast<test_val_type>(node_cap); ++v) {
      auto slice_k = slice_make_from(k);
      auto slice_v = slice_make_from(v);
      n.insert(slice_k, slice_v);
    }
  }
}