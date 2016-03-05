
#include "search.h"

#include "../base/testing.h"

namespace search {

TEST(binarysearch_empty) {
  std::vector<int> arr = {};
  ASSERT_EQ(-1, BinarySearch(arr, 0));
}

TEST(binarysearch_notfound) {
  std::vector<int> arr = {1, 2, 3, 7};
  ASSERT_EQ(-1, BinarySearch(arr, 0));
  ASSERT_EQ(-1, BinarySearch(arr, 5));
}

TEST(binarysearch_found_even) {
  std::vector<int> arr = {1, 2, 3, 4};
  ASSERT_EQ(0, BinarySearch(arr, 1));
  ASSERT_EQ(2, BinarySearch(arr, 3));
  ASSERT_EQ(3, BinarySearch(arr, 4));
}

TEST(binarysearch_found_odd) {
  std::vector<int> arr = {1, 3, 4};
  ASSERT_EQ(0, BinarySearch(arr, 1));
  ASSERT_EQ(1, BinarySearch(arr, 3));
  ASSERT_EQ(2, BinarySearch(arr, 4));
}

TEST(binarysearch_found_duplicates) {
  std::vector<int> arr = {1, 3, 3, 3, 3, 4};
  ASSERT_LT(0, BinarySearch(arr, 3));
}

TEST(binarysearch_greater) {
  std::vector<int> arr = {4, 3, 1};
  {
    int ix = BinarySearch<int, std::greater<int>>(arr, 1);
    ASSERT_EQ(2, ix);
  }
  {
    int ix = BinarySearch<int, std::greater<int>>(arr, 0);
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
  bool operator()(const TestValue& a, const TestValue& b) {
    if (a.x != b.x) {
      return a.x < b.x;
    }
    return a.y < b.y;
  }
};

struct TestEq {
  bool operator()(const TestValue& a, const TestValue& b) {
    return a.x == b.x && a.y == b.y;
  }
};

TEST(binarysearch_obj) {
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
    int ix = BinarySearch<TestValue, TestComp, TestEq>(arr, TestValue(0, 1));
    ASSERT_EQ(1, ix);
  }
  {
    int ix = BinarySearch<TestValue, TestComp, TestEq>(arr, TestValue(1, 8));
    ASSERT_EQ(-1, ix);
  }
}

}  // namespace search
