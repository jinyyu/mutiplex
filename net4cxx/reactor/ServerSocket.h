#ifndef NET4CXX_DISTRIBUTION_SOCKET_H
#define NET4CXX_DISTRIBUTION_SOCKET_H

#include <net4cxx/common/Status.h>
#include <net4cxx/common/NonCopyable.h>


namespace net4cxx
{

class InetSocketAddress;

class ServerSocket: NonCopyable
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

#endif
