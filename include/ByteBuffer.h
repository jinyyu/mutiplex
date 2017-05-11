#ifndef LIBNET_DISTRIBUTION_BYTEBUFFER_H
#define LIBNET_DISTRIBUTION_BYTEBUFFER_H
#include <inttypes.h>
#include <vector>
#include "NonCopyable.h"

namespace net
{


class ByteBuffer : NonCopyable {

 public:
  ByteBuffer() : position_(0), mark_(0) {
    limit_ = buff_.size();
  }
  void put(char* data, uint32_t len);
  int remaining() { return limit_ - position_; }
  bool has_remaining() { return limit_ > position_ ; }
  void reset() { position_ = mark_; }
  void rewind() {position_ = mark_ = 0; }

  void flip()
  {
    limit_ = position_;
    position_ = 0;
  }






 private:
  uint32_t position_;
  uint32_t mark_;
  uint32_t limit_;
  std::vector<char> buff_;
};


}

#endif //LIBNET_DISTRIBUTION_BYTEBUFFER_H
