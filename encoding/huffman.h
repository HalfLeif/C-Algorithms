#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace huffman {

// TODO Implement functions for
// * decoding
// * taking sample of text -> distribution.
// * storing and loading code book

// Main program should be able to
// * Read text, produce distribution, output code book to stdout.
// * Load code book from file, encode stdin > stdout.
// * Load code book from file, decode stdin > stdout.

class Huffman {
 public:
  // TODO build error string as member?
  Huffman(std::vector<float> distribution, const std::vector<unsigned char>& tokens);
  Huffman(std::vector<float> distribution, const std::string& tokens);

  // Must implement deep copy if we want to copy this class.
  Huffman(const Huffman&) = delete;

  bool is_successful() const { return successful_; }

  // Number of known tokens. Does not include the Escape token.
  size_t VocabularySize() const { return token_map_.size(); }

  // Returns a string of 0 and 1.
  // If token was not present in input, will return code for Escape token.
  std::string Code(unsigned char token) const;

  // If token is unknown, returns EscapeCode() + token.
  // Otherwise, simply returns Code(token).
  std::string Encoded(unsigned char token) const;

  // Code used for Escaping unknown unsigned chars.
  // Will always be the longest code.
  std::string EscapeCode() const;

  bool Encode(std::istream* input, std::ostream* output) const;
  // bool Decode(std::istream* input, std::ostream* output) const;

  /*
  std::string Encode(const std::string& plain_text) const;
  std::string Decode(const std::string& encoded_text) const;
  */

  // Internal
  struct HuffmanNode {
    HuffmanNode* parent = nullptr;
    HuffmanNode* zero = nullptr;
    HuffmanNode* one = nullptr;
    // Redundant information but makes encoding slightly faster and easier to read:
    bool is_one = false;
  };

 private:
  // Will give index of Escape token if unknown token.
  size_t IndexOfToken(unsigned char token) const;

  // Returns code as string of 1 and 0 for this leaf.
  std::string CodeInternal(size_t leaf) const;

  bool successful_;
  std::vector<HuffmanNode> nodes_;

  // Map of tokens to index in nodes_ and distribution_
  std::map<unsigned char,size_t> token_map_;
};

std::vector<float> EnglishLetterDistribution();

}  // namespace huffman

#endif
