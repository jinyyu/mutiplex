#pragma once

#include <boost/noncopyable.hpp>
#include <evcpp/callbacks.h>
#include <evcpp/ServerSocket.h>
#include <evcpp/InetSocketAddress.h>

namespace ev
{


class EventSource;
class EventLoop;

class Acceptor: boost::noncopyable
{
public:
    Acceptor(EventLoop* loop, int port);

    Acceptor(EventLoop* loop, const InetSocketAddress& addr);

    ~Acceptor();

    void new_connection_callback(const NewConnectionCallback& callback)
    {
        on_new_connection_ = callback;
    }

private:
    EventLoop* loop_;
    ServerSocket server_socket_;
    EventSource* accept_event_;
    InetSocketAddress peer_addr_;
    InetSocketAddress local_addr_;
    NewConnectionCallback on_new_connection_;
};

}

