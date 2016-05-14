
#include "huffman.h"

#include <algorithm>
#include <queue>
#include <utility>

#include "../base/logging.h"

using Node = huffman::Huffman::HuffmanNode;
using QueueElem = std::pair<float, size_t>;

namespace huffman {
namespace {

bool IsLeaf(const Node& node) {
  return node.zero == nullptr && node.one == nullptr;
}

bool IsRoot(const Node& node) {
  return node.parent == nullptr;
}

bool IsParentConsistent(const Node& node) {
  if (node.parent == &node) return false;
  if (node.is_one) return !IsRoot(node) && node.parent->one == &node;
  return IsRoot(node) || &node == node.parent->zero || &node == node.parent->one;
}

bool AreChildrenConsistent(const Node& node) {
  if (node.zero && node.zero->parent != &node) return false; 
  if (node.one && node.one->parent != &node) return false; 
  return true;
}

bool IsConsistent(const std::vector<Node>& tree, size_t book_size) {
  if (tree.size() != 2 * book_size - 1) {
    return false;
  }
  for (size_t i = 0; i < tree.size(); ++i) {
    if (!IsParentConsistent(tree[i])) return false;
    if (!AreChildrenConsistent(tree[i])) return false;

    bool expect_leaf = i < book_size;
    if (expect_leaf != IsLeaf(tree[i])) return false;

    bool expect_root = i == tree.size() - 1;
    if (expect_root != IsRoot(tree[i])) return false;
  }
  return true;
}

// Returns false on failure.
// If success, `out` will hold 2N-1 elements where the first N elements occupy the leaves of the tree.
// The char for distribution[i] has its Node at (*out)[i].
// The last element of out is the root of the tree.
bool ConstructHuffmanNodes(const std::vector<float>& distribution, std::vector<Node>* out) {
  if (out == nullptr) {
    return false;
  }

  const size_t book_size = distribution.size();
  const size_t tree_size = 2 * book_size;

  // Allocates memory for the binary tree
  out->resize(tree_size - 1);

  // Queue of <weight, tree_index>. Queue should output smallest weight first.
  std::priority_queue<QueueElem, std::vector<QueueElem>, std::greater<QueueElem>> queue;

  // Insert all leaves into the queue
  for (size_t i = 0; i < distribution.size(); ++i) {
    queue.emplace(distribution[i], i);
  }

  size_t next_empty_node = book_size;
  while(queue.size() > 1) {
    // Create a new parent node for the two trees with current smallest weight.
    QueueElem lower = queue.top();
    queue.pop();
    QueueElem low = queue.top();
    queue.pop();

    Node* lower_node = &(*out)[lower.second];
    Node* low_node = &(*out)[low.second];

    // Should never happen because we insert one for every two we remove:
    CHECK(next_empty_node < out->size());
    Node* parent = &(*out)[next_empty_node];

    lower_node->parent = parent;
    low_node->parent = parent;
    parent->zero = low_node;
    parent->one = lower_node;
    lower_node->is_one = true;

    queue.emplace(lower.first + low.first, next_empty_node);
    ++next_empty_node;
  }
  return IsConsistent(*out, book_size);
}

// Distribution of English letters in percentage.
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

std::vector<char> VectorFromString(const std::string& str) {
  std::vector<char> result;
  for (char c : str) {
    result.push_back(c);
  }
  return result;
}

}  // namespace

Huffman::Huffman(std::vector<float> distribution, const std::string& tokens)
    : Huffman(std::move(distribution), VectorFromString(tokens)) {}

Huffman::Huffman(std::vector<float> distribution, const std::vector<char>& tokens) {
  if (distribution.size() != tokens.size()) {
    successful_ = false;
    return;
  }
  for (size_t i = 0; i < tokens.size(); ++i) {
    token_map_.emplace(tokens[i], i);
  }
  distribution.push_back(0.0f);  // for Escape token.
  successful_ = ConstructHuffmanNodes(distribution, &nodes_);
}

std::string Huffman::CodeInternal(size_t leaf_index) const {
  std::string result;
  for (const Node* node = &nodes_[leaf_index]; !IsRoot(*node); node = node->parent) {
    if (node->is_one) {
      result.push_back('1');
    } else {
      result.push_back('0');
    }
  }
  std::reverse(result.begin(), result.end());
  return result;
}

std::string Huffman::EscapeCode() const {
  return CodeInternal(Booksize());
}

std::string Huffman::Code(char token) const {
  auto it = token_map_.find(token);
  if (it == token_map_.end()) return EscapeCode();
  return CodeInternal(it->second);
}

std::vector<float> EnglishLetterDistribution() {
  std::vector<float> result;
  for (size_t i = 0; i < 26; ++i) {
    result.push_back(kEnglishLetterDistribution[i] / 100.0f);
  }
  return result;
}

}  // namespace huffman
