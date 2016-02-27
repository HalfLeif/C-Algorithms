#ifndef CONTAINER_UTILS_H
#define CONTAINER_UTILS_H

namespace util {

template <typename A, typename B>
struct PairHasher {
 public:
  size_t operator() (const std::pair<A, B>& pair) const {
    return hash_a_(pair.first) + 32503 * hash_b_(pair.second);
  }
 private:
  std::hash<A> hash_a_;
  std::hash<B> hash_b_;
};

template <typename Container, typename Key>
bool ContainsKey(const Container& container, const Key& key) {
  return container.find(key) != container.end();
}

template <typename OuterContainer, typename T>
bool ContainsInElem(const OuterContainer& sets, const T& key) {
  for (const auto& set : sets) {
    if (ContainsKey(set, key)) {
      return true;
    }
  }
  return false;
}

}  // util

#endif
