
#include "huffman.h"

#include <algorithm>
#include <queue>
#include <utility>

#include "../base/logging.h"

namespace huffman {

namespace {

std::vector<std::string> ConstructCodes(const std::vector<int>& index_tree,
                                        const std::vector<bool>& orientation,
                                        size_t book_size) {
  std::vector<std::string> result;
  for (size_t i = book_size; i < index_tree.size(); ++i) {
    std::string code;
    // TODO check off by one error here!
    for (size_t j = i; j > 1; j = index_tree[j]) {
      if (orientation[j]) {
        code.push_back('1');
      } else  {
        code.push_back('0');
      }
    }
    std::reverse(code.begin(), code.end());
    result.push_back(std::move(code));
  }
  return result;
}

}  // namespace

typedef std::pair<float, int> QueueElem;

std::vector<std::string> HuffmanCodes(const std::vector<float>& distribution) {
  const size_t book_size = distribution.size();
  const size_t tree_size = 2 * book_size;

  // Vector of zeroes, will be used to represent our binary tree.
  std::vector<int> index_tree(tree_size, 0);
  std::vector<bool> orientation(tree_size, false);

  // Queue of <weight, tree_index>. Queue should output smallest weight first.
  std::priority_queue<QueueElem, std::vector<QueueElem>, std::greater<QueueElem>> queue;

  // Put weight of leaves at the end half of the vector
  // Insert all leaves into the queue
  std::vector<float> weights(tree_size);
  for (size_t i = 0; i < book_size; ++i) {
    weights[i + book_size] = distribution[i];
    queue.emplace(distribution[i], i + book_size);
  }

  // Create a new parent node for the two trees with current smallest weight.
  size_t next_empty_node = book_size - 1;
  while(queue.size() > 1) {
    auto lower = queue.top();
    queue.pop();
    auto low = queue.top();
    queue.pop();

    // Create parent node
    index_tree[lower.second] = next_empty_node;
    index_tree[low.second] = next_empty_node;
    orientation[lower.second] = true;

    float parent_weight = lower.first + low.first;
    weights[next_empty_node] = parent_weight;

    queue.emplace(parent_weight, next_empty_node);

    // Should never happen because we insert one for every two we remove:
    CHECK(next_empty_node > 0);
    --next_empty_node;
  }

  return ConstructCodes(index_tree, orientation, book_size);
}

static const float kEnglishLetterDistribution[] = {
  8.167,   // a
  1.492,   // b
  2.782,   // c
  4.253,   // d
  12.702,  // e
  2.228,   // f
  2.015,   // g
  6.094,   // h
  6.966,   // i
  0.153,   // j
  0.772,   // k
  4.025,   // l
  2.406,   // m
  6.749,   // n
  7.507,   // o
  1.929,   // p
  0.095,   // q
  5.987,   // r
  6.327,   // s
  9.056,   // t
  2.758,   // u
  0.978,   // v
  2.361,   // w
  0.150,   // x
  1.974,   // y
  0.074,   // z
};

std::vector<float> EnglishLetterDistribution() {
  std::vector<float> result;
  for (size_t i = 0; i < 26; ++i) {
    result.push_back(kEnglishLetterDistribution[i] / 100.0f);
  }
  return result;
}

}  // namespace huffman
