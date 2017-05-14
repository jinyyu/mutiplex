#ifndef LIBNET_DISTRIBUTION_BYTEBUFFER_H
#define LIBNET_DISTRIBUTION_BYTEBUFFER_H
#include <inttypes.h>
#include <vector>
#include "NonCopyable.h"
#include "Status.h"

namespace net
{

class ByteBuffer : NonCopyable
{
 public:
  ByteBuffer() : position_(0), mark_(-1), limit_(buff_.size()) {}
  ByteBuffer(int init_size) : position_(0), mark_(-1), limit_(init_size), buff_(init_size) {}

  int position() { return position_; }
  Status position(int p);

  int limit() { return limit_; }
  void limit(int limit ) {}

  void clear();


 private:

 private:

  int limit_;
  int mark_;
  int position_;
  std::vector<char> buff_;

};

}

#endif //LIBNET_DISTRIBUTION_BYTEBUFFER_H
