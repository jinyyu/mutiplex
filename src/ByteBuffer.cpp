#include <string.h>
#include "ByteBuffer.h"
#include "Exception.h"
#include <assert.h>
namespace net
{

ByteBuffer::ByteBuffer(int capacity)
    : position_(0),
      limit_(capacity),
      capacity_(capacity),
      mark_(-1)
{
  data_ = (char*) malloc(capacity);
}

ByteBuffer::~ByteBuffer()
{
  free(data_);
}

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
  if (limit <= capacity_ && limit >= 0) {
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
  limit_ = capacity_;
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

void ByteBuffer::put(void* data, int len)
{
  if (len > remaining()) {
    throw Exception::invalid_argument(nullptr);
  }  else {
    memcpy(this->data(), data, len);
    position_ += len;
  }
  assert(position_ <= limit_);
}

void ByteBuffer::get(void* buffer, int len)
{
  if (len > remaining()) {
    throw Exception::invalid_argument(nullptr);
  } else {
    memcpy(buffer, data(), len);
    position_ += len;
  }
  assert(position_ <= limit_);
}


}