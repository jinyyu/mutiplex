#pragma once
#include <boost/noncopyable.hpp>
#include <memory>
#include <evcpp/InetAddress.h>
#include <evcpp/callbacks.h>

#include <deque>
#include <evcpp/ByteBuffer.h>

namespace ev
{

class EventSource;
class EventLoop;
class CircularBuffer;
class ByteBuffer;

class Connection: public std::enable_shared_from_this<Connection>, boost::noncopyable
{
public:
    explicit Connection(int fd,
                        EventLoop* loop,
                        const InetAddress& local,
                        const InetAddress& peer);

    ~Connection();

    void set_buffer_size(uint32_t size)
    {
        buffer_size_ = size;
    }


    InetAddress local_address() const
    {
        return local_;
    }

    InetAddress peer_address() const
    {
        return peer_;
    }

    int local_port() const
    {
        return local_.port();
    }

    int peer_port() const
    {
        return peer_.port();
    }

    int fd() const
    {
        return fd_;
    }

    void set_established_callback(const EstablishedCallback& cb)
    {
        established_callback_ = cb;
    }

    void set_read_callback(const ReadCallback& cb)
    {
        read_callback_ = cb;
    }

    void set_closed_callback(const ClosedCallback& cb)
    {
        closed_callback_ = cb;
    }

    void error_callback(const ErrorCallback& cb)
    {
        error_callback_ = cb;
    }

    void close();

    void force_close();

    bool is_closed()
    {
        return state_ == Closed || state_ == Disconnecting;
    }

    bool write(const void* data, uint32_t len);

    bool write(const ByteBufferPtr& buffer);

    EventLoop* loop() const
    {
        return loop_;
    }

private:
    friend class EventLoop;
    void register_event();

    void do_write(const void* data, uint32_t len);

    void handle_write(uint64_t timestamp);

    bool has_bytes_to_write() const;
private:
    enum State
    {
        New = 0,
        Receiving = 1,
        Disconnecting = 2,
        Closed = 3

    };

    int fd_;
    uint8_t state_;
    uint32_t buffer_size_;
    EventLoop* loop_;
    EventSource* event_source_;
    InetAddress local_;
    InetAddress peer_;

    EstablishedCallback established_callback_;
    ReadCallback read_callback_;
    ClosedCallback closed_callback_;
    ErrorCallback error_callback_;

    friend class CircularBuffer;
    CircularBuffer* buffer_out_;
};

}

