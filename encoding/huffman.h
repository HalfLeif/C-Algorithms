#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <vector>
#include <string>

namespace huffman {

// Computes optimal huffman codes as strings of 0 and 1 for the given distribution.
// Order of output is the same as order of input.
std::vector<std::string> HuffmanCodes(const std::vector<float>& distribution);

std::vector<float> EnglishLetterDistribution();

}  // namespace huffman

#endif
