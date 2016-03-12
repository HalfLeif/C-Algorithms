
#include "../base/testing.h"

// Note: can overflow!
int UnsafeIsRoot(uint r, uint x) {
  if (r*r > x) {
    // r is too large
    return 1;
  }
  ++r;
  if (r*r > x) {
    // Found it!
    return 0;
  }
  // r is too small
  return -1;
}

// Safe against overflow
int IsRoot(uint r, uint x) {
  if (r == 0) {
    if (x == 0) return 0;
    return -1;
  }
  uint d = x / r;
  if (r > d) {
    // r is too large
    return 1;
  }
  ++r;
  d = x / r;
  if (r > d) {
    // Found it!
    return 0;
  }
  // r is too small
  return -1;
}

// Returns floor of sq root
uint SquareRoot(const uint x) {
  if (x < 2) {
    return x;
  }

  // inclusive ranges:
  uint low = 1;
  uint high = x/2;

  while (low <= high) {
    // Avoid overflow:
    uint mid = low + (high - low) / 2;
    int is_found = IsRoot(mid, x);
    if (is_found == 0) {
      return mid;
    } else if (is_found > 0) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  // Should never happen
  LOG(ERROR) << "Square root not found!! for x " << x << "\nLow: " << low << "\nHigh: " << high;
  return 0;
}

TEST(is_root_true) {
  ASSERT_EQ(0, IsRoot(3, 9));
  ASSERT_EQ(0, IsRoot(3, 10));
  ASSERT_EQ(0, IsRoot(3, 11));
  ASSERT_EQ(0, IsRoot(2, 8));
  ASSERT_EQ(0, IsRoot(2, 4));
  ASSERT_EQ(0, IsRoot(1, 3));
  ASSERT_EQ(0, IsRoot(0, 0));
}

TEST(is_root_too_high) {
  ASSERT_EQ(1, IsRoot(4, 9));
  ASSERT_EQ(1, IsRoot(4, 10));
  ASSERT_EQ(1, IsRoot(8, 11));
  ASSERT_EQ(1, IsRoot(3, 8));
  ASSERT_EQ(1, IsRoot(2, 3));
  ASSERT_EQ(1, IsRoot(1, 0));
}

TEST(is_root_too_low) {
  ASSERT_EQ(-1, IsRoot(2, 9));
  ASSERT_EQ(-1, IsRoot(2, 10));
  ASSERT_EQ(-1, IsRoot(4, 25));
  ASSERT_EQ(-1, IsRoot(4, 26));
  ASSERT_EQ(-1, IsRoot(5, 36));
  ASSERT_EQ(-1, IsRoot(0, 1));
}

TEST(find_root) {
  ASSERT_EQ(0, SquareRoot(0));
  ASSERT_EQ(1, SquareRoot(2));
  ASSERT_EQ(1, SquareRoot(3));
  ASSERT_EQ(2, SquareRoot(4));
  ASSERT_EQ(2, SquareRoot(8));
  ASSERT_EQ(3, SquareRoot(9));
  ASSERT_EQ(5, SquareRoot(25));
  ASSERT_EQ(5, SquareRoot(35));
  ASSERT_EQ(6, SquareRoot(36));
}

TEST(root_overflow) {
  uint large = 1 << 30;
  uint root = 1 << 15;
  ASSERT_EQ(0, IsRoot(root, large));
  ASSERT_EQ(root, SquareRoot(large));
}

TEST(div_assumption) {
  for (uint r = 1; r < 100; ++r) {
    for (uint x = 1; x < 200; x += 1) {
      uint d = x / r;
      bool left_side = r * r > x;
      bool right_side = r > d;
      ASSERT_EQ(left_side, right_side) << "r: " << r << " x: " << x;
    }
  }
}
