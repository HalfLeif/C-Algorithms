#include "bitstream.h"

#include <cstdio>
#include <sstream>
#include <string>

#include "../base/testing.h"

typedef std::ostringstream os_stream;

namespace bitstream {
namespace {
void DebugOutput(const os_stream& output) {
  if (output.str().empty()) {
    LOG(INFO) << "Empty output";
  } else {
    LOG(INFO) << "Output bytes: ";
    for (unsigned char c : output.str()) {
      printf("%x ", c);
    }
    printf("\n");
    LOG(INFO) << "Has " << output.str().size() << " bytes.";
  }
}
}  // namespace

TEST(empty_input) {
  os_stream output;
  ASSERT_EMPTY(output.str());

  BitOutStreamer bit_stream(&output);
  bit_stream.FlushRemaining();

  ASSERT_EMPTY(output.str());
  ASSERT_EMPTY(bit_stream);
}

TEST(even_bytes_identity) {
  const std::string input = "abcdef";
  os_stream output;
  ASSERT_EMPTY(output.str());

  BitOutStreamer bit_stream(&output);
  for (char c : input) {
    bit_stream.PushByte(c);
  }
  bit_stream.FlushRemaining();

  ASSERT_EQ(input, output.str());
  ASSERT_EMPTY(bit_stream);
}

TEST(even_bits_identity) {
  ASSERT_EQ('a', 0x61);  // 0x61 == 0110 0001
  
  std::vector<int> input = {0, 1, 1, 0,
                            0, 0, 0, 1};
  os_stream output;
  ASSERT_EMPTY(output.str());

  BitOutStreamer bit_stream(&output);
  for (int i : input) {
    bit_stream.PushBit(i == 1);
  }
  bit_stream.FlushRemaining();

  ASSERT_FALSE(output.str().empty());

  ASSERT_EQ("a", output.str());
  ASSERT_EMPTY(bit_stream);
}

TEST(odd_bits_followed_by_zeroes) {
  ASSERT_EQ('a', 0x61);  // 0x61 == 0110 0001
  ASSERT_EQ('h', 0x68);  // 0x68 == 0110 1000

  std::vector<int> input = {0, 1, 1, 0,
                            0, 0, 0, 1,
                            0, 1, 1, 0,
                            1};  // Fill rest with zeroes
  os_stream output;
  ASSERT_EMPTY(output.str());

  BitOutStreamer bit_stream(&output);
  for (int i : input) {
    bit_stream.PushBit(i == 1);
  }
  bit_stream.FlushRemaining();

  ASSERT_FALSE(output.str().empty());

  ASSERT_EQ("ah", output.str());
  ASSERT_EMPTY(bit_stream);
}

TEST(outputs_when_full) {
  os_stream output;
  ASSERT_EMPTY(output.str());
  BitOutStreamer bit_stream(&output);

  bit_stream.SetBufferSize(5);
  const std::string four = "1234";
  // Note that c : "1234" will also iterate through terminating NIL char \000.
  for (char c : four) {
    bit_stream.PushByte(c);
  }
  ASSERT_EMPTY(output.str()) << output.str();

  bit_stream.PushBit(false);
  bit_stream.PushByte('\xFF');
  // Buffer will now output but leave this one trailing bit in the buffer.

  ASSERT_FALSE(bit_stream.empty());
  ASSERT_EQ("1234\x7F", output.str());

  bit_stream.FlushRemaining();
  ASSERT_EMPTY(bit_stream);
  ASSERT_EQ("1234\x7F\x80", output.str());
}

TEST(flushes_on_destr) {
  os_stream output;
  ASSERT_EMPTY(output.str());

  const std::string input = "abcdefg";
  {
    BitOutStreamer bit_stream(&output);
    for (char c : input) {
      bit_stream.PushByte(c);
    }
    // bit_stream gets destroyed as it falls out of scope.
    // Flushes in destructor.
  }
  ASSERT_EQ(input, output.str());
}

}
