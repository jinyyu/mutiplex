#ifndef NET4CXX_DISTRIBUTION_SESSION_H
#define NET4CXX_DISTRIBUTION_SESSION_H

#include <net4cxx/common/callbacks.h>
#include <net4cxx/common/InetSocketAddress.h>

namespace reactor
{

class EventLoop;
class InetSocketAddress;
class Channel;

class Session
{
public:
    explicit Session(EventLoop* loop, const InetSocketAddress& local);

    ~Session();

    void connect_error_callback(const ConnectErrorCallback& cb)
    { connect_error_callback_ = cb; }

    void connection_established_callback(const ConnectionEstablishedCallback& cb)
    { connection_established_callback_ = cb; }

    void read_message_callback(const ReadMessageCallback& cb)
    { read_message_callback_ = cb; }

    void connection_closed_callback(const ConnectionClosedCallback& cb)
    { connection_closed_callback_ = cb; }

    void connect(const InetSocketAddress& peer);

private:
    void handle_connected(const Timestamp& timestamp, SelectionKey* key);

    bool do_connect(const InetSocketAddress& addr);

    void handle_error(const Timestamp& timestamp);

private:
    int fd_;
    EventLoop* loop_;
    InetSocketAddress local_;
    InetSocketAddress peer_;
    Channel* channel_;

    ConnectErrorCallback connect_error_callback_;
    ConnectionEstablishedCallback connection_established_callback_;
    ReadMessageCallback read_message_callback_;
    ConnectionClosedCallback connection_closed_callback_;
};

}


#endif
