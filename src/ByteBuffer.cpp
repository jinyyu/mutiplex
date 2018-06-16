#include <string.h>
#include <netinet/in.h>
#include "net4cxx/common/ByteBuffer.h"
#include <stdlib.h>
#include <assert.h>
#include <log4cxx/logger.h>


namespace reactor
{

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

ByteBuffer::ByteBuffer(int capacity)
    : position_(0),
      limit_(capacity),
      mark_(-1)
{
    if (capacity < 0) {
        LOG4CXX_ERROR(logger, "invalid capacity " << capacity);
    }
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
    if (p <= limit_ && p >= 0) {
        position_ = p;
        if (mark_ > position_) {
            mark_ = -1;
        }
    }
    else {
        LOG4CXX_ERROR(logger, "invalid position " << p);
    }
}

void ByteBuffer::limit(int limit)
{
    if (limit <= capacity() && limit >= 0) {
        limit_ = limit;
        if (position_ > limit_) {
            position_ = limit_;
        }

        if (mark_ > limit_) {
            mark_ = -1;
        }
    }
    else {
        LOG4CXX_ERROR(logger, "invalid limit " << limit);
    }
}

void ByteBuffer::reset()
{
    if (mark_ < 0) {
        LOG4CXX_ERROR(logger, "invalid mark " << mark_);
    }
    else {
        position_ = mark_;
    }
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
    if (len > remaining()) {
        LOG4CXX_ERROR(logger, "len > remaining, len = " << len << " remaining " << remaining());
    }
    else {
        memcpy(this->data() , data, len);
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
    assert(remaining() >= n);
    position_ += n;
}

}