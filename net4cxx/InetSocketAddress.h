#ifndef NET4CXX_DISTRIBUTION_INETSOCKETADDRESS_H
#define NET4CXX_DISTRIBUTION_INETSOCKETADDRESS_H
#include <net4cxx/InetAddress.h>

namespace net4cxx
{

class InetSocketAddress
{
public:
    InetSocketAddress();

    //Creates a socket address from an IP address and a port number
    InetSocketAddress(const InetAddress &addr, int port);

    //Creates a socket address where the IP address is the wildcard address and the port number a specified value
    InetSocketAddress(int port);

    //Creates a socket address from a hostname and a port number
    InetSocketAddress(const char *hostname, int port);

    //Gets the InetAddress
    InetAddress get_address() const;

    //Gets the port number
    int port() const;

    int family() const
    {
        sockaddr *addr = (sockaddr *) &sockaddr6_;
        return addr->sa_family;
    }

    std::string to_string() const;

private:
    friend class ServerSocket;
    friend class Session;
    sockaddr *sockaddr_cast() const
    { return (sockaddr *) &sockaddr6_; }

    void from_address(const InetAddress &addr, int port);

    union
    {
        struct sockaddr_in sockaddr_;
        struct sockaddr_in6 sockaddr6_;
    };

};

}


#endif //LIBNET_DISTRIBUTION_INETSOCKETADDRESS_H
