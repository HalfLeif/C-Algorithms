#ifndef HUFFMAN_H
#define HUFFMAN_H

// #include <iostream>
#include <map>
#include <string>
#include <vector>

namespace huffman {

// TODO
// std::vector<std::pair<char, float>> CalculateDistribution(const std::string& sample);

class Huffman {
 public:
  // TODO build error string as member?
  Huffman(std::vector<float> distribution, const std::vector<char>& tokens);
  Huffman(std::vector<float> distribution, const std::string& tokens);

  // TODO implement deep copy if we want this.
  Huffman(const Huffman&) = delete;

  bool is_successful() const { return successful_; }

  // Returns empty string for unknown token, otherwise a string of 0 and 1.
  // Mostly useful for debugging and testing.
  std::string Code(char token) const;

  // bool Encode(std::istream* input, std::ostream* output) const;

  /*
  std::string Encode(const std::string& plain_text) const;
  std::string Decode(const std::string& encoded_text) const;
  */

  // Internal
  struct HuffmanNode {
    HuffmanNode* parent = nullptr;
    HuffmanNode* zero = nullptr;
    HuffmanNode* one = nullptr;
    bool is_one = false;
  };

 private:
  bool successful_;
  std::vector<HuffmanNode> nodes_;

  // Map of tokens to index in nodes_ and distribution_
  std::map<char,size_t> token_map_;
};

std::vector<float> EnglishLetterDistribution();

}  // namespace huffman

#endif
