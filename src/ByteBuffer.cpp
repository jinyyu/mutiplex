#include <string.h>
#include "ByteBuffer.h"
#include "Logger.h"
#include <stdlib.h>

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
    LOG_ERROR("invalid position %d", p);
  }
}

void ByteBuffer::limit(int limit)
{
  if (limit <= capacity_ && limit >= 0) {
    limit_ = limit;
    if (position_ > limit_) {
      position_ = limit_;
    }

    if (mark_ > limit_) {
      mark_ = -1;
    }
  } else {
    LOG_ERROR("invalid limit %d", limit);
  }
}

void ByteBuffer::reset()
{
  if (mark_ < 0) {
    LOG_ERROR("invalid mark %d", mark_);
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
    LOG_ERROR("len > remaining, len = %d, remaining = %d", len, remaining());
  }  else {
    memcpy(this->data(), data, len);
    position_ += len;
  }
}

int ByteBuffer::get(void* buffer, int len)
{
  len = std::min(len, remaining());
  memcpy(buffer, data(), len);
  position_ += len;
  return len;
}

void ByteBuffer::skip(int n)
{
  if (remaining() < n) {
    LOG_ERROR("n > remaining");
  }

  position_ += n;
}

}