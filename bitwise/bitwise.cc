
#include "bitwise.h"
#include "../base/logging.h"

namespace bitwise {
namespace {
// Only takes positive values
int MultPositive(const int a, int other) {
  if (other > a) {
    return MultPositive(other, a);
  }

  int result = 0;
  const int bits = sizeof(int) * 8;
  const int leftmost = 1 << (bits - 1);
  for (int i = 0; i < bits; ++i) {
    int bit = leftmost & other;
    if (bit != 0) {
      result += a;
    }
    if (i < bits - 1) {
      other <<= 1;
      result <<= 1;
    }
  }
  return result;
}
}  // namespace

int Mult(int a, int other) {
  int sign = 1;
  if (a < 0) {
    a *= -1;
    sign *= -1;
  }
  if (other < 0) {
    other *= -1;
    sign *= -1;
  }
  return sign * MultPositive(a, other);
}

}  // namespace bitwise
