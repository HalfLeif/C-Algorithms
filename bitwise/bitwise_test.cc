
#include "bitwise.h"

#include <cstdlib>

#include "../base/testing.h"

int RandInt(int begin, int end) {
  if (begin >= end) {
    LOG(ERROR) << "Must give a true interval!";
  }
  const int interval = end - begin;
  return begin + rand() % interval;
}

TEST(rand_test) {
  srand(0);

  for (int i = 0; i < 100; ++i) {
    int a = RandInt(-5, 10);
    ASSERT_LT(a, 10);
    ASSERT_GE(a, -5);
  }
}

TEST(bitwise_mult_zero_test) {
  srand(0);

  int a = 0;
  for (int i = 0; i < 100; ++i) {
    int b = RandInt(-5, 5);
    ASSERT_EQ(0, bitwise::Mult(a, b));
  }
}

TEST(bitwise_mult_pos_test) {
  srand(0);

  for (int i = 0; i < 100; ++i) {
    int a = RandInt(1, 10);
    int b = RandInt(1, 10);
    ASSERT_EQ(a*b, bitwise::Mult(a, b));
  }
}

TEST(bitwise_mult_neg_test) {
  srand(0);

  for (int i = 0; i < 100; ++i) {
    int a = RandInt(-10, 0);
    int b = RandInt(-5, 10);
    ASSERT_EQ(a*b, bitwise::Mult(a, b));
  }
}

