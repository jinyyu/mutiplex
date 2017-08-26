#ifndef LIBNET_DISTRIBUTION_CIRCULARBUFFER_H
#define LIBNET_DISTRIBUTION_CIRCULARBUFFER_H

#include <libnet/NonCopyable.h>
#include <inttypes.h>

namespace net
{
class Timestamp;
class Connection;

class CircularBuffer: NonCopyable
{
public:
    explicit CircularBuffer(uint32_t capacity);

    ~CircularBuffer();

    //Returns this buffer's capacity
    uint32_t capacity() const
    { return capacity_; }

    void put(const void *buffer, uint32_t length);

    uint32_t get(void *buffer, uint32_t length);

    uint32_t buffer_len() const
    { return in_ - out_; }

    bool empty() const
    { return in_ == out_; }

    bool full() const
    { return in_ - out_ == capacity_; }

    void clear()
    { in_ = out_ = 0; }

private:

    uint32_t buffer_remaining() const
    { return capacity_ - in_ + out_; }

    void resize(const void *buffer, uint32_t length);

    uint32_t out() const
    { return out_ & (capacity_ - 1); }

    uint32_t in() const
    { return in_ & (capacity_ - 1); }

    friend class Connection;
    int write_to_fd(Connection *conn, const Timestamp &timestamp);
private:
    char *data_;
    uint32_t capacity_;
    uint32_t in_;
    uint32_t out_;
};

}


#endif //LIBNET_DISTRIBUTION_CIRCULARBUFFER_H
