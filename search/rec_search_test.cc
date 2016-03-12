
#include "search.h"

#include "../base/testing.h"

namespace search {

TEST(rec_binarysearch_empty) {
  std::vector<int> arr = {};
  ASSERT_EQ(-1, RecursiveBinarySearch(arr, 0));
}

TEST(rec_binarysearch_notfound) {
  std::vector<int> arr = {1, 2, 3, 7};
  ASSERT_EQ(-1, RecursiveBinarySearch(arr, 0));
  ASSERT_EQ(-1, RecursiveBinarySearch(arr, 5));
  ASSERT_EQ(-1, RecursiveBinarySearch(arr, 8));
}

TEST(rec_binarysearch_found_even) {
  std::vector<int> arr = {1, 2, 3, 4};
  ASSERT_EQ(0, RecursiveBinarySearch(arr, 1));
  ASSERT_EQ(2, RecursiveBinarySearch(arr, 3));
  ASSERT_EQ(3, RecursiveBinarySearch(arr, 4));
}

TEST(rec_binarysearch_found_odd) {
  std::vector<int> arr = {1, 3, 4};
  ASSERT_EQ(0, RecursiveBinarySearch(arr, 1));
  ASSERT_EQ(1, RecursiveBinarySearch(arr, 3));
  ASSERT_EQ(2, RecursiveBinarySearch(arr, 4));
}

TEST(rec_binarysearch_found_duplicates) {
  std::vector<int> arr = {1, 3, 3, 3, 3, 4};
  ASSERT_LT(0, RecursiveBinarySearch(arr, 3));
}

TEST(rec_binarysearch_greater) {
  std::vector<int> arr = {4, 3, 1};
  {
    int ix = RecursiveBinarySearch<int, std::greater<int>>(arr, 1);
    ASSERT_EQ(2, ix);
  }
  {
    int ix = RecursiveBinarySearch<int, std::greater<int>>(arr, 0);
    ASSERT_EQ(-1, ix);
  }
}

struct TestValue {
  TestValue(int a, int b) : x(a), y(b) {}
  int x = 0;
  int y = 0;
  int ignore = 0;
};

struct TestComp {
  bool operator()(const TestValue& a, const TestValue& b) const {
    if (a.x != b.x) {
      return a.x < b.x;
    }
    return a.y < b.y;
  }
};

struct TestEq {
  bool operator()(const TestValue& a, const TestValue& b) const {
    return a.x == b.x && a.y == b.y;
  }
};

TEST(rec_binarysearch_obj) {
  std::vector<TestValue> arr;
  arr.emplace_back(0, 0);
  arr.emplace_back(0, 1);
  arr.emplace_back(1, 0);
  arr.emplace_back(1, 1);
  arr.emplace_back(1, 1);
  arr.emplace_back(1, 1);
  arr.emplace_back(1, 1);
  arr.emplace_back(1, 2);
  arr.emplace_back(1, 3);
  arr.emplace_back(2, 3);
  {
    int ix = RecursiveBinarySearch<TestValue, TestComp, TestEq>(arr, TestValue(0, 1));
    ASSERT_EQ(1, ix);
  }
  {
    int ix = RecursiveBinarySearch<TestValue, TestComp, TestEq>(arr, TestValue(1, 8));
    ASSERT_EQ(-1, ix);
  }
}

}  // namespace search
