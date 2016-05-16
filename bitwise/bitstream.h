#ifndef BITSTREAM_H_
#define BITSTREAM_H_

#include <iostream>
#include <queue>

namespace bitstream {

// Operates on bit level, useful for compression where each token may be of different bit length.
//
// Example:
// 110, 00, 111011, FLUSH -> 1100 0111 0110 0000
class BitOutStreamer {
 public:
  explicit BitOutStreamer(std::ostream* out);
  ~BitOutStreamer() { FlushRemaining(); };

  // Flush remaining bits. If the content is not evenly divisable by 8,
  // will output the last bits as most significant bits followed by zeros.
  // Will always be empty after flushing.
  //
  // Example:
  // 1101 1001 110 -> 1101 1001 1100 0000
  void FlushRemaining();

  // Number of bytes to keep in buffer.
  // Must be larger than zero.
  // Will output every time the buffer size is exceeded.
  void SetBufferSize(size_t size);

  void PushBit(bool bit);
  void PushByte(unsigned char byte);

  // Returns true iff the buffer is empty.
  bool empty() const { return buffer_.empty(); }

 private:
  bool IsTimeToOutput() const;

  std::queue<bool> buffer_;
  std::ostream* out_stream_;  // Not owned.
  size_t max_buffer_size_ = 10;  // In bytes.
};

// TODO also make a BitInStreamer ?

}  // namespace bitstream

#endif
