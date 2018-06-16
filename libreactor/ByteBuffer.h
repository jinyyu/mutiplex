#pragma once

#include <boost/noncopyable.hpp>
#include <boost/asio/detail/shared_ptr.hpp>

namespace reactor
{

class ByteBuffer: boost::noncopyable
{
public:
    explicit ByteBuffer(int capacity);

    ~ByteBuffer();

    //Returns this buffer's capacity
    int capacity() const;

    //Returns this buffer's position
    int position() const
    { return position_; }

    //Sets this buffer's position
    void position(int p);

    //Returns this buffer's limit
    int limit() const
    { return limit_; }

    //Sets this buffer's limit
    void limit(int limit);

    //Sets this buffer's mark at its position
    void mark()
    { mark_ = position_; }

    //Resets this buffer's position to the previously-marked position
    //Invoking this method neither changes nor discards the mark's value
    void reset();

    //Clears this buffer. The position is set to zero, the limit is set to the capacity, and the mark is discarded
    //Invoke this method before using a sequence of channel-read or put operations to fill this buffer
    void clear();

    //Flips this buffer. The limit is set to the current position and then the position is set to zero. If the mark is defined then it is discarded
    void flip();

    //Rewinds this buffer. The position is set to zero and the mark is discarded
    void rewind();

    void skip(int n);

    //Returns the number of elements between the current position and the limit
    int remaining() const
    { return limit_ - position_; }

    //Tells whether there are any elements between the current position and the limit
    bool has_remaining() const
    { return position_ < limit_; }

    const void* data() const
    { return data_ + sizeof(int) + position_; }

    void* data()
    { return data_ + sizeof(int) + position_; }

    //bulk get method
    int get(void* buffer, int len);

    //bulk put method
    void put(const void* data, int len);

private:
/*
| capacity 4 bytes  |  data bytes      |
*/
    int limit_;
    int mark_;
    int position_;
    char* data_;
};

typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

}

