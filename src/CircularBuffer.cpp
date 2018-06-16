#include "libreactor/CircularBuffer.h"
#include "libreactor/utils.h"
#include "libreactor/Connection.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <unistd.h>
#include <log4cxx/logger.h>

namespace reactor
{

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

CircularBuffer::CircularBuffer(uint32_t capacity)
    : in_(0),
      out_(0)
{
    uint32_t cap = capacity > 0x80000000 ? 0x80000000 : capacity; //2^31

    if (is_pow_of_two(cap)) {
        capacity_ = cap;
    }
    else {
        capacity_ = roundup_pow_of_two(cap);
    }

    data_ = (char*) malloc(capacity_);
}

CircularBuffer::~CircularBuffer()
{
    free(data_);
}

void CircularBuffer::put(const void* buffer, uint32_t length)
{
    if (buffer_remaining() < length) {
        resize(buffer, length);
        return;
    }
    else {
        //length <= buffer_remaining()
        uint32_t in_index = in();

        /* first put the data starting from in_ to buffer end */
        uint32_t len = std::min(length, capacity_ - in_index);
        memcpy(data_ + in_index, buffer, len);

        /* then put the rest (if any) at the beginning of the buffer */
        memcpy(data_, (char*) buffer + len, length - len);

        in_ += length;
    }
}

uint32_t CircularBuffer::get(void* buffer, uint32_t length)
{
    uint32_t size = std::min(length, buffer_len());

    uint32_t out_index = out();

    /* first get the data from out_ until the end of the buffer */
    uint32_t len = std::min(size, capacity_ - out_index);
    memcpy(buffer, data_ + out_index, len);

    /* then get the rest (if any) from the beginning of the buffer */
    memcpy((char*) buffer + len, data_, size - len);

    out_ += size;
    return size;
}

void CircularBuffer::resize(const void* buffer, uint32_t length)
{
    uint32_t cap = roundup_pow_of_two(length + capacity_);
    char* data = (char*) malloc(cap);

    uint32_t buff_len = buffer_len();
    uint32_t out_index = out();
    uint32_t in_index = in();

    if (full() && in_index == 0) {
        memcpy(data, data_, capacity_);
    }
    else {
        uint32_t len = std::min(buff_len, capacity_ - out_index);

        // first put the data starting from in_ to buffer end
        memcpy(data, data_ + out_index, len);

        // then put the rest (if any) from the beginning of the buffer to out_
        memcpy(data + len, data_, buff_len - len);

        //last append data to the buffer
        memcpy(data + buff_len, buffer, length);
    }


    free(data_);
    data_ = data;

    out_ = 0;
    in_ = length + buff_len;
    capacity_ = cap;
}

int CircularBuffer::write_to_fd(Connection* conn, const Timestamp& timestamp)
{
    if (empty()) {
        LOG4CXX_ERROR(logger, "buffer is empty");
        return -1;
    }

    uint32_t buff_len = buffer_len();

    uint32_t out_index = out();
    uint32_t in_index = in();

    ssize_t n = -1;
    if (full() && in_index == 0) {
        //case out_ == (n) capacity_, in_ == (n+1) capacity_
        n = ::write(conn->fd_, data_, capacity_);
        if (n < 0) {
            LOG4CXX_ERROR(logger, "write error " << errno);
            return n;
        }
    }
    else {
        uint32_t len = std::min(buff_len, capacity_ - out_index);

        struct iovec iov[2];
        iov[0].iov_base = data_ + out_index;
        iov[0].iov_len = len;

        iov[1].iov_base = data_;
        iov[1].iov_len = buff_len - len;

        n = ::writev(conn->fd_, iov, (iov[1].iov_len == 0) ? 1 : 2);
        if (n < 0) {
            LOG4CXX_ERROR(logger, "writev error fd = " << conn->fd_ << " error = " << errno);
            return n;
        }
    }
    out_ += static_cast<uint32_t>(n);
    return static_cast<int>(n);
}

}
