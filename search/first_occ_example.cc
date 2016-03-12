
#include <vector>

#include "../base/testing.h"

using std::vector;

// Returns 0 on found
// -1 on too small i
// +1 on too large i
int FoundSpot(const vector<int>& v, const size_t i, const int value) {
  if (i >= v.size() || v[i] > value) {
    return 1;
  }
  if (v[i] == value) {
    if (i == 0 || v[i-1] < value) {
      return 0;
    }
    return 1;
  }
  return -1;
}

static const size_t npos = -1;

// Find first occurrence of value in sorted array
size_t FindFirstOcc(const vector<int>& v, const int value) {
  size_t low = 0;
  size_t end = v.size();
  while (low < end) {
    size_t mid = (low + end) / 2;
    int found = FoundSpot(v, mid, value);
    if (found == 0) {
      return mid;
    } else if (found == 1) {
      end = mid;
    } else {
      low = mid + 1;
    }
  }
  return npos;
}

TEST(firstocc_findspot) {
  std::vector<int> v = {1, 2, 3, 5, 8};
  ASSERT_EQ(0, FoundSpot(v, 0, 1));
  ASSERT_EQ(0, FoundSpot(v, 1, 2));
  ASSERT_EQ(0, FoundSpot(v, 4, 8));

  ASSERT_EQ(1, FoundSpot(v, 3, 2));
  ASSERT_EQ(1, FoundSpot(v, 5, 8));
}

TEST(firstocc_not_found) {
  std::vector<int> v = {1, 2, 3, 5, 8};
  ASSERT_EQ(npos, FindFirstOcc(v, 0));
  ASSERT_EQ(npos, FindFirstOcc(v, 4));
  ASSERT_EQ(npos, FindFirstOcc(v, 7));
  ASSERT_EQ(npos, FindFirstOcc(v, 9));
}

TEST(firstocc_multiple) {
  std::vector<int> v = {1, 2, 2, 2, 2, 3, 3, 3, 5, 8, 8, 8, 8};
  ASSERT_EQ(0, FindFirstOcc(v, 1));
  ASSERT_EQ(1, FindFirstOcc(v, 2));
  ASSERT_EQ(5, FindFirstOcc(v, 3));
  ASSERT_EQ(9, FindFirstOcc(v, 8));
}


