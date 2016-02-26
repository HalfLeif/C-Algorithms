
#include "base/testing.h"

namespace {

TEST(integer_test) {
  ASSERT_TRUE(true);
  ASSERT_FALSE(false);

  ASSERT_EQ(1, 1);
  ASSERT_NE(1, 0);

  ASSERT_LT(0, 1);
  ASSERT_LE(0, 1);
  ASSERT_LE(0, 0);

  ASSERT_GT(2, 1);
  ASSERT_GE(2, 1);
  ASSERT_GE(2, 2);
}

TEST(fail_test){
  int a = 0;
  ASSERT_LT(1, a);
}

}  // namespace
