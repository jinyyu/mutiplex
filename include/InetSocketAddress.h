#ifndef LIBNET_DISTRIBUTION_INETSOCKETADDRESS_H
#define LIBNET_DISTRIBUTION_INETSOCKETADDRESS_H
#include "InetAddress.h"

namespace net
{

class InetSocketAddress
{
public:
  InetSocketAddress() {}

  //Creates a socket address from an IP address and a port number
  InetSocketAddress(const InetAddress& addr, int port);

  //Creates a socket address where the IP address is the wildcard address and the port number a specified value
  InetSocketAddress(int port);

  //Creates a socket address from a hostname and a port number
  InetSocketAddress(const char* hostname, int port);

  //Gets the InetAddress
  InetAddress get_address() const;

  //Gets the port number
  int port() const;

  int family() const {
    sockaddr* addr = (sockaddr*)&sockaddr6_;
    return addr->sa_family;
  }

private:
  friend class ServerSocket;
  sockaddr* sockaddr_cast() const { return (sockaddr*)&sockaddr6_; }

  void from_address(const InetAddress& addr, int port);

  union {
    struct sockaddr_in sockaddr_;
    struct sockaddr_in6 sockaddr6_;
  };


};

}



#endif //LIBNET_DISTRIBUTION_INETSOCKETADDRESS_H
