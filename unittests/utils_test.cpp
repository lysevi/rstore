#include <rstore/utils/strings.h>
#include <rstore/utils/utils.h>
#include <rstore/utils/cz.h>

#include "helpers.h"
#include <array>
#include <catch.hpp>
#include <numeric>

TEST_CASE("utils.split") {
  std::array<int, 8> tst_a;
  std::iota(tst_a.begin(), tst_a.end(), 1);

  std::string str = "1 2 3 4 5 6 7 8";
  auto splitted_s = rstore::utils::strings::tokens(str);

  std::vector<int> splitted(splitted_s.size());
  std::transform(splitted_s.begin(),
                 splitted_s.end(),
                 splitted.begin(),
                 [](const std::string &s) { return std::stoi(s); });

  EXPECT_EQ(splitted.size(), size_t(8));

  bool is_equal
      = std::equal(tst_a.begin(), tst_a.end(), splitted.begin(), splitted.end());
  EXPECT_TRUE(is_equal);
}

TEST_CASE("utils.to_upper") {
  auto s = "lower string";
  auto res = rstore::utils::strings::to_upper(s);
  EXPECT_EQ(res, "LOWER STRING");
}

TEST_CASE("utils.to_lower") {
  auto s = "UPPER STRING";
  auto res = rstore::utils::strings::to_lower(s);
  EXPECT_EQ(res, "upper string");
}

void f_throw() {
  throw rstore::utils::exceptions::Exception("error");
}

TEST_CASE("utils.exception") {
  try {
    f_throw();
  } catch (rstore::utils::exceptions::Exception &e) {
    rstore::utils::logging::logger_info(e.what());
  }
}

TEST_CASE("utils.clz") {
  EXPECT_EQ(rstore::utils::clz(67553994410557440), 8);
  EXPECT_EQ(rstore::utils::clz(3458764513820540928), 2);
  EXPECT_EQ(rstore::utils::clz(15), 60);

  EXPECT_EQ(rstore::utils::ctz(240), 4);
  EXPECT_EQ(rstore::utils::ctz(3840), 8);
}