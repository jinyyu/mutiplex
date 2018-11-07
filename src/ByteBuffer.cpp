#include <string.h>
#include <netinet/in.h>
#include "mutiplex/ByteBuffer.h"
#include "Debug.h"
#include <stdlib.h>
#include <assert.h>


namespace muti
{

ByteBuffer::ByteBuffer(int capacity)
    : position_(0),
      limit_(capacity),
      mark_(-1)
{
    assert(capacity > 0);
    int cap = htonl(capacity);
    data_ = (char*) malloc(capacity + sizeof(int));
    memcpy(data_, &cap, sizeof(int));
}

ByteBuffer::~ByteBuffer()
{
    free(data_);
}

int ByteBuffer::capacity() const
{
    int cap;
    memcpy(&cap, data_, sizeof(int));
    return ntohl(cap);
}

void ByteBuffer::position(int p)
{
    assert(p <= limit_ && p >= 0);
    position_ = p;
    if (mark_ > position_) {
        mark_ = -1;
    }
}

void ByteBuffer::limit(int limit)
{
    assert(limit <= capacity() && limit >= 0);
    limit_ = limit;
    if (position_ > limit_) {
        position_ = limit_;
    }

    if (mark_ > limit_) {
        mark_ = -1;
    }
}

void ByteBuffer::reset()
{
    assert(mark_ >= 0);
    position_ = mark_;
}

void ByteBuffer::clear()
{
    position_ = 0;
    limit_ = capacity();
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

void ByteBuffer::put(const void* data, int len)
{
    assert(len <= remaining());

    memcpy(this->data(), data, len);
    position_ += len;

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
    assert(remaining() >= n);
    position_ += n;
}

}