#pragma once

#include <boost/noncopyable.hpp>
#include <evcpp/callbacks.h>
#include <evcpp/ServerSocket.h>
#include <evcpp/InetAddress.h>

namespace ev
{

class EventSource;
class EventLoop;

class Acceptor: boost::noncopyable
{
public:
    Acceptor(EventLoop* loop, const std::string& addr_str);

    Acceptor(EventLoop* loop, const InetAddress& addr);

    ~Acceptor();

    void new_connection_callback(const NewConnectionCallback& callback)
    {
        on_new_connection_ = callback;
    }

private:
    EventLoop* loop_;
    ServerSocket server_socket_;
    EventSource* accept_event_;
    InetAddress peer_addr_;
    InetAddress local_addr_;
    NewConnectionCallback on_new_connection_;
};

}

