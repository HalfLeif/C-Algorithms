
#include "edit_distance.h"

#include <algorithm>
#include <unordered_map>

#include "../base/logging.h"
#include <iostream>
#include <string>

// Assumes all strings are null terminated
namespace strings {
struct PairHasher {
  size_t operator() (const std::pair<const char*, const char*>& p) const {
    return size_t(p.first) + 32503 * size_t(p.second);
  }
};
typedef std::unordered_map<std::pair<const char*, const char*>, int, strings::PairHasher> Matrix;

namespace {
int Length(const char* str) {
  if (str == nullptr) return 0;
  const char* const begin = str;
  while (*str != '\0') {
    ++str;
  }
  return str - begin;
}

// Always returns the remaining cost
int InternalEditDistance(const char* a, const char* b, Matrix* matrix) {
  std::pair<const char*, const char*> pair = std::make_pair(a, b);
  auto it = matrix->find(pair);
  if (it != matrix->end()) {
    return it->second;
  }
  int result = -1;
  if (b == nullptr || *b == '\0') {
    result = Length(a);
  } else if (a == nullptr || *a == '\0') {
    result = Length(b);
  } else if (*a == *b) {
    result = InternalEditDistance(a + 1, b + 1, matrix);
  } else {
    const int replace = 1 + InternalEditDistance(a + 1, b + 1, matrix);
    const int delete_a = 1 + InternalEditDistance(a + 1, b, matrix);
    const int delete_b = 1 + InternalEditDistance(a, b + 1, matrix);
    result = std::min(replace, std::min(delete_a, delete_b));
  }
  matrix->insert(std::make_pair(pair, result));
  return result;
}
}  // namespace

int EditDistance(const char* a, const char* b) {
  Matrix matrix;
  return InternalEditDistance(a, b, &matrix);
}

int BruteEditDistance(const char* a, const char* b) {
  if (b == nullptr || *b == '\0') {
    return Length(a);
  }
  if (a == nullptr || *a == '\0') {
    return Length(b);
  }
  if (*a == *b) {
    return BruteEditDistance(a + 1, b + 1);
  }
  const int replace = 1 + BruteEditDistance(a + 1, b + 1);
  const int delete_a = 1 + BruteEditDistance(a + 1, b);
  const int delete_b = 1 + BruteEditDistance(a, b + 1);
  return std::min(replace, std::min(delete_a, delete_b));
}

}  // namespace strings

