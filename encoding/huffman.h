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
  Huffman(std::vector<float> distribution, std::vector<char> tokens);

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

  // Contains one float for each token AND one additional element for the Escape token.
  // Escape tokens are used to escape characters not present in the token vector.
  std::vector<float> distribution_;

  // TODO Represent as std::map<char,size_t> instead?
  std::map<char,size_t> token_map_;
};

/*
// Computes optimal huffman codes as strings of 0 and 1 for the given distribution.
// Order of output is the same as order of input.
std::vector<std::string> HuffmanCodes(const std::vector<float>& distribution);
*/

std::vector<float> EnglishLetterDistribution();

}  // namespace huffman

#endif
