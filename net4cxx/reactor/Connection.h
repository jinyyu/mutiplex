#ifndef NET4CXX_DISTRIBUTION_CONNECTION_H
#define NET4CXX_DISTRIBUTION_CONNECTION_H
#include <net4cxx/common/NonCopyable.h>
#include <memory>

#include <net4cxx/common/InetSocketAddress.h>
#include <net4cxx/common/InetAddress.h>
#include <net4cxx/reactor/callbacks.h>
#include <net4cxx/reactor/Context.h>


#include <deque>
#include "net4cxx/common/ByteBuffer.h"

namespace net4cxx
{
class EventLoop;
class Selector;
class Channel;
class CircularBuffer;
class ByteBuffer;

class Connection: public std::enable_shared_from_this<Connection>, NonCopyable
{
public:
    explicit Connection(int fd,
                        EventLoop* loop,
                        const InetSocketAddress& local,
                        const InetSocketAddress& peer);

    ~Connection();

    void set_buffer_size(uint32_t size)
    { buffer_size_ = size; }

    const InetSocketAddress& local_socket_address() const
    { return local_; }

    const InetSocketAddress& peer_socket_address() const
    { return peer_; }

    InetAddress local_address() const
    { return local_.get_address(); }

    InetAddress peer_address() const
    { return peer_.get_address(); }

    int local_port() const
    { return local_.port(); }

    int peer_port() const
    { return peer_.port(); }

    int fd() const
    { return fd_; }

    void connection_established_callback(const ConnectionEstablishedCallback& cb)
    { connection_established_callback_ = cb; }

    void read_message_callback(const ReadMessageCallback& cb)
    { read_message_callback_ = cb; }

    void connection_closed_callback(const ConnectionClosedCallback& cb)
    { connection_closed_callback_ = cb; }

    void error_callback(const ErrorCallback& cb)
    { error_callback_ = cb; }

    void close();

    void force_close();

    bool is_closed()
    { return state_ == Closed || state_ == Disconnecting; }

    bool write(const void* data, uint32_t len);

    bool write(const ByteBufferPtr& buffer);

    EventLoop* loop() const
    { return loop_; }

    ContextPtr& context()
    { return ctx_; }

    void context(ContextPtr& ctx)
    { ctx_ = std::move(ctx); }

private:
    friend class EventLoop;
    void setup_callbacks();

    void do_write(const void* data, uint32_t len);

    void handle_write(const Timestamp& timestamp);

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
    Channel* channel_;
    InetSocketAddress local_;
    InetSocketAddress peer_;

    ConnectionEstablishedCallback connection_established_callback_;
    ReadMessageCallback read_message_callback_;
    ConnectionClosedCallback connection_closed_callback_;
    ErrorCallback error_callback_;

    friend class CircularBuffer;
    CircularBuffer* buffer_out_;
    ContextPtr ctx_;
};

}

#endif
