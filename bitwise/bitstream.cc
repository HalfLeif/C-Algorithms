
#include "bitstream.h"

#include "../base/logging.h"

typedef std::queue<bool> BitBuffer;
typedef unsigned char uchar;

namespace bitstream {
namespace internal {

void PushByteToBuffer(uchar byte, BitBuffer* out) {
  const static uchar left_most = 128;
  for (int i = 0; i < 8; ++i) {
    out->push(byte & left_most);
    byte = byte << 1;
  }
}

// Buffer will always contain less than 8 bits after this operation.
void OutputRaw(BitBuffer* buffer, std::ostream* output) {
  while (buffer->size() >= 8) {
    uchar out_byte = '\000';
    for(int i = 0; i < 8; ++i) {
      out_byte = out_byte << 1;
      bool is_one = buffer->front();
      buffer->pop();
      if (is_one) {
        out_byte |= '\001';
      }
    }
    (*output) << out_byte;
  }
}

void FlushRemaining(BitBuffer* buffer, std::ostream* output) {
  OutputRaw(buffer, output);
  // Now there is less than 8 bits left in the buffer.

  // If buffer contained even 8 bits, return.
  if (buffer->empty()) return;

  // Output remaining bits followed by zeroes.
  uchar out_byte = '\000';
  int bits_left = 8;
  while (!buffer->empty()) {
    out_byte = out_byte << 1;
    bool is_one = buffer->front();
    buffer->pop();
    if (is_one) {
      out_byte |= '\001';
    }
    --bits_left;
  }
  if (bits_left <= 0) {
    // Should never happen.
    LOG(ERROR) << "While flushing, unexpected bits left: " << bits_left;
    return;
  }
  out_byte = out_byte << bits_left;
  (*output) << out_byte;
}

}  // namespace internal

BitOutStreamer::BitOutStreamer(std::ostream* output) : out_stream_(output) {}

void BitOutStreamer::FlushRemaining() {
  internal::FlushRemaining(&buffer_, out_stream_);
}

void BitOutStreamer::SetBufferSize(size_t size) {
  if (size < 1) return;
  max_buffer_size_ = size;
}

bool BitOutStreamer::IsTimeToOutput() const {
  size_t max_bits = 8 * max_buffer_size_;
  return buffer_.size() >= max_bits;
}

void BitOutStreamer::PushBit(bool bit) {
  buffer_.push(bit);
  if (IsTimeToOutput()) internal::OutputRaw(&buffer_, out_stream_);
}

void BitOutStreamer::PushByte(uchar byte) {
  for (int i = 7; i >= 0; --i) {
    uchar bit_position = 1 << i;
    buffer_.push(byte & bit_position);
  }
  if (IsTimeToOutput()) internal::OutputRaw(&buffer_, out_stream_);
}

}  // namespace bitstream


