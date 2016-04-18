#include "queue.h"
#include "../base/testing.h"

namespace queue {

TEST(capacity) {
  Queue<int> q;
  ASSERT_EQ(0, q.size());
  ASSERT_TRUE(q.empty());
  q.reserve(5);
  ASSERT_EQ(5, q.capacity());
  q.reserve(30);
  ASSERT_EQ(30, q.capacity());
}

TEST(back) {
  Queue<int> q;
  q.push_back(255);
  ASSERT_EQ(1, q.size());
  ASSERT_EQ(255, q.back());
  ASSERT_EQ(255, q.front());

  for(int i = 0; i < 100; ++i) {
    q.push_back(i);
    ASSERT_EQ(i, q.back());
    ASSERT_EQ(255, q.front());
  }
  ASSERT_EQ(101, q.size());

  ASSERT_EQ(99, q.back());
  ASSERT_EQ(99, q.pop_back());
  ASSERT_EQ(98, q.back());
}

TEST(front) {
  Queue<int> q;
  q.push_front(255);
  ASSERT_EQ(1, q.size());
  ASSERT_EQ(255, q.front());
  ASSERT_EQ(255, q.back());

  for(int i = 0; i < 100; ++i) {
    q.push_front(i);
    ASSERT_EQ(i, q.front());
    ASSERT_EQ(255, q.back());
  }
  ASSERT_EQ(101, q.size());

  ASSERT_EQ(99, q.front());
  ASSERT_EQ(99, q.pop_front());
  ASSERT_EQ(98, q.front());
}

TEST(reserve_change) {
  Queue<int> q;
  q.push_front(255);
  q.push_front(300);
  ASSERT_EQ(255, q.back());
  ASSERT_EQ(300, q.front());

  q.reserve(4);
  ASSERT_EQ(255, q.back());
  ASSERT_EQ(300, q.front());
}

TEST(multi_reserve_back) {
  Queue<int> q;
  q.reserve(2);
  for (int i = 55; i < 59; ++i) {
    q.push_back(i);
    ASSERT_EQ(55, q.front()) << "\n\t" << i;
    ASSERT_EQ(i, q.back());
  }
}

TEST(iter) {
  Queue<int> q;
  for (int i = 0; i < 20; ++i) {
    q.push_front(i);
  }
  int ix = 0;
  for (int elem : q) {
    ASSERT_EQ(elem, ix);
    ++ix;
  } 
}

TEST(doesnt_invalidate_iterators_with_deletion) {
  Queue<int> q;
  for (int i = 0; i < 20; ++i) {
    q.push_front(i);
  }
  int ix = 0;
  for (int elem : q) {
    if (ix > 15) break;
    if (ix == 12) {
      q.clear();
    }
    ++ix;
  }
  // Ie, exits loop immediately after clear.
  ASSERT_EQ(13, ix);
}

TEST(iter_value_preserved) {
  Queue<int> q;
  q.push_front(1);
  q.push_front(2);
  q.push_front(3);
  q.push_front(4);

  auto iter = q.begin();
  ++iter;
  ASSERT_EQ(2, *iter);

  // Unchanged after pop front
  q.pop_front();
  ASSERT_EQ(2, *iter);

  // Keeps position relative to begin().
  q.pop_back();
  ASSERT_EQ(3, *iter);
  ASSERT_EQ(2, *q.begin());
}

TEST(shrink) {
  Queue<int> q;
  q.reserve(30);
  for (int i = 1; i <= 14; ++i) {
    q.push_front(i);
  }
  q.shrink_to_fit();
  ASSERT_EQ(14, q.size());
  ASSERT_EQ(1, q.back());
  ASSERT_EQ(14, q.front());

  int ix = 1;
  for (int elem : q) {
    ASSERT_EQ(elem, ix);
    ++ix;
  }
}

}  // namespace queue
