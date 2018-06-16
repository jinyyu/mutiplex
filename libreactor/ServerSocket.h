#pragma once

#include <libreactor/Status.h>
#include <boost/noncopyable.hpp>

namespace reactor
{

class InetSocketAddress;

class ServerSocket: boost::noncopyable
{
public:
    //Creates an unbound server socket
    explicit ServerSocket();

    ~ServerSocket();

    //Binds the ServerSocket to a specific address (IP address and port number)
    Status bind(const InetSocketAddress& addr);

    int accept(InetSocketAddress& addr);

    //Enable/disable the SO_REUSEADDR socket option
    void reuse_address(bool on);

    void reuse_port(bool on);

    int fd() const
    { return fd_; }

private:
    void listen();

    int fd_;
};

}

