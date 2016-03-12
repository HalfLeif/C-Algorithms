#include <functional>
#include <vector>

namespace search {

template <class T, class Comp, class Eq>
size_t BinarySearch(const std::vector<T>& arr, const T& value);

template <class T, class Comp, class Eq>
size_t RecursiveBinarySearch(const std::vector<T>& arr, const T& value);

// Implementation --------------------

template <class T, class Comp = std::less<T>, class Eq = std::equal_to<T>>
size_t BinarySearch(const std::vector<T>& arr, const T& value) {
  Comp comparator;
  Eq equals;
  size_t low = 0;
  size_t end = arr.size();
 
  while (low < end) {
    size_t mid = (low + end) / 2;
    if (equals(arr[mid], value)) {
      return mid;
    } else if (comparator(arr[mid], value)) {
      low = mid + 1;
    } else {
      end = mid;
    }
  }
  return -1;
}

namespace internal {
template <class T, class Comp, class Eq>
size_t TailBinarySearch(const std::vector<T>& arr, const T& value,
                             const Comp& comparator, const Eq& equals,
                             size_t low, size_t end) {
  if (low >= end) {
    return -1;
  }
  size_t mid = (low + end) / 2;
  if (equals(arr[mid], value)) {
    return mid;
  } else if (comparator(arr[mid], value)) {
    return TailBinarySearch(arr, value, comparator, equals, mid + 1, end);
  } else {
    return TailBinarySearch(arr, value, comparator, equals, low, mid);
  }
}
}  // namespace internal

template <class T, class Comp = std::less<T>, class Eq = std::equal_to<T>>
size_t RecursiveBinarySearch(const std::vector<T>& arr, const T& value) {
  Comp comparator;
  Eq equals;
  return internal::TailBinarySearch(arr, value, comparator, equals, 0, arr.size());
}
}
