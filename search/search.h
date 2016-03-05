#include <functional>
#include <vector>

namespace search {

template <class T, class Comp, class Eq>
size_t BinarySearch(const std::vector<T>& arr, const T& value);

// Implementation --------------------

template <class T, class Comp = std::less<T>, class Eq = std::equal_to<T>>
size_t BinarySearch(const std::vector<T>& arr, const T& value) {
  if (arr.empty()) return -1;

  Comp comparator;
  Eq equals;
  size_t low = 0;
  size_t end = arr.size();

  if (comparator(value, arr[low])) { return -1;}
  if (comparator(arr[end - 1], value)) { return -1;}

  while (low < end) {
    size_t mid = (low + end) / 2;
    if (equals(arr[mid], value)) {
      return mid;
    } else if (low == mid) {
      return -1;
    } else if (comparator(arr[mid], value)) {
      low = mid;
    } else {
      end = mid;
    }
  }
  return -1;
}
}
