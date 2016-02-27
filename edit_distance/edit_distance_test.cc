
#include "edit_distance.h"

#include "../base/testing.h"

namespace {
const char kEmpty[] = "";
const char kAaa[] = "aaaaa";
const char kZaa[] = "zaaaaa";
const char kXaa[] = "xaaaaa";
const char kXax[] = "xxxaaaaaxx";

TEST(edit_distance_test) {
  ASSERT_EQ(0, strings::EditDistance(kEmpty, kEmpty));
  ASSERT_EQ(1, strings::EditDistance(kEmpty, "a"));
  ASSERT_EQ(2, strings::EditDistance(kEmpty, "aa"));

  ASSERT_EQ(0, strings::EditDistance(kAaa, kAaa));
  ASSERT_EQ(1, strings::EditDistance(kAaa, kZaa));
  ASSERT_EQ(1, strings::EditDistance(kXaa, kZaa));
  ASSERT_EQ(4, strings::EditDistance(kXaa, kXax));
  ASSERT_EQ(5, strings::EditDistance(kZaa, kXax));
}

const char kShorter[] = "blah blah x";
const char kShort[] = "hej blah x blah";

TEST(edit_distance_comparison_test) {
  ASSERT_EQ(strings::BruteEditDistance(kShorter, kShort), strings::EditDistance(kShorter, kShort));
}
}  // namespace
