#ifndef LIBNET_DISTRIBUTION_INETADDRESS_H
#define LIBNET_DISTRIBUTION_INETADDRESS_H
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>

namespace net
{

class Status;

class InetAddress {

public:
  InetAddress(const InetAddress &address)
  {
    this->is_v4_ = address.is_v4_;
    memcpy(&addr_, &address.addr_, sizeof(addr_));
  }

  InetAddress &operator=(const InetAddress& address)
  {
    this->is_v4_ = address.is_v4_;
    memcpy(&addr_, &address.addr_, sizeof(addr_));
    return *this;
  }

  bool v4() const { return is_v4_; }

  bool v6() const { return !is_v4_; }

  //Checks whether the address has been resolved or not
  bool is_resolved();

  enum Family {
    INET = 1,
    INET6 = 2
  };

  std::string to_string() const;

  //No name service is checked for the validity of the address
  static InetAddress get_by_address(const char* addr, Family family = INET);

  static InetAddress any(Family family = INET);

  //Determines the IP address of a host, given the host's name
  static InetAddress get_by_host(const char* hostname, Status& status);

private:
  InetAddress() { memset(&addr_, 0, sizeof(addr_)); }

private:
  bool is_v4_;
  in6_addr addr_;

};


}

#endif //LIBNET_DISTRIBUTION_INETADDRESS_H
