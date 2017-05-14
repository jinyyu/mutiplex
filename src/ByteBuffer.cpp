#include <string.h>
#include "ByteBuffer.h"
#include "Exception.h"
namespace net
{

void ByteBuffer::position(int p)
{
  if (p <= limit_ && p >= 0) {
    position_ = p;
    if (mark_ > position_) {
      mark_ = -1;
    }
  } else {
    throw Exception::invalid_argument("invalid position");
  }
}

void ByteBuffer::limit(int limit) {
  if (limit <= buff_.size() && limit >= 0) {
    limit_ = limit;
    if (position_ > limit_) {
      position_ = limit_;
    }

    if (mark_ > limit_) {
      mark_ = -1;
    }
  } else {
    throw Exception::invalid_argument("invalid position");
  }
}

void ByteBuffer::reset()
{
  if (mark_ < 0) {
    throw Exception::invalid_argument("invalid mark");
  } else {
    position_ = mark_;
  }
}

void ByteBuffer::clear()
{
  position_ = 0;
  limit_ = buff_.size();
  mark_ = -1;
}

void ByteBuffer::flip()
{
  limit_ = position_;
  position_ = 0;
  mark_ = -1;
}

void ByteBuffer::rewind()
{
  position_ = 0;
  mark_ = -1;
}

void ByteBuffer::skip_bytes(int n)
{
  if (remaining() < n) {
    throw Exception::invalid_argument(nullptr);
  }
  position_ += n;


}


}