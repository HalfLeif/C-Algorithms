
#include "huffman.h"

#include <cstdio>
#include <sstream>

#include "../base/container_utils.h"
#include "../base/testing.h"
#include "../bitwise/bitstream.h"

typedef std::istringstream is_stream;
typedef std::ostringstream os_stream;

namespace huffman {

// Returns string of '1' and '0' for the encoding of the plaintext.
std::string EncodedBits(const Huffman& huff, const std::string& plaintext) {
  std::string bits;
  for (unsigned char c : plaintext) {
    bits += huff.Encoded(c);
  }
  return bits;
}

// Produce expected result for encoding huffman.
// Considerably slower than the original implementation but should be equivalent.
//
// Also handles escaping of unknown chars.
std::string ProduceEquivalent(const Huffman& huff, const std::string& plaintext) {
  std::string codes = EncodedBits(huff, plaintext);
  os_stream string_stream;
  bitstream::BitOutStreamer bit_stream(&string_stream);
  for (unsigned char bit : codes) {
    bit_stream.PushBit(bit == '1');
  }
  bit_stream.FlushRemaining();
  return string_stream.str();
}

void PrintHex(const std::string& raw) {
  for (unsigned char c : raw) {
    printf("%x ", c);
  }
  printf("\n");
}

TEST(huffman_empty_encode) {
  const Huffman huff({}, "");
  ASSERT_TRUE(huff.is_successful());
  ASSERT_EMPTY(huff.EscapeCode());

  // Empty huffman has empty escape code so will just return all chars unchanged.

  const std::string input_str = "Many chars represented!\t #indeed, @hi./?\\";
  is_stream input(input_str);
  os_stream output;
  ASSERT_EMPTY(output.str());

  huff.Encode(&input, &output);
  ASSERT_EQ(input_str, output.str());
}

TEST(huffman_encode_simple) {
  const Huffman huff({0.5, 0.1, 0.3, 0.4}, "abcd");
  ASSERT_TRUE(huff.is_successful());

  const std::string input_str = "bbbacddc";
  is_stream input(input_str);
  os_stream output;
  ASSERT_EMPTY(output.str());

  huff.Encode(&input, &output);

  std::string expected = ProduceEquivalent(huff, input_str);
  ASSERT_EQ(expected, output.str());
}

TEST(huffman_encode_escaping) {
  const Huffman huff({0.5, 0.1, 0.3, 0.4}, "abcd");
  ASSERT_TRUE(huff.is_successful());

  const std::string input_str = "bb b a cddc.";
  is_stream input(input_str);
  os_stream output;
  ASSERT_EMPTY(output.str());

  huff.Encode(&input, &output);

  std::string expected = ProduceEquivalent(huff, input_str);
  ASSERT_EQ(expected, output.str());
}

}  // namespace huffman

