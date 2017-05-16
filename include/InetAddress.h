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
    addr6_ = address.addr6_;
  }

  bool operator== (const InetAddress& address);

  bool operator!= (const InetAddress& address);

  InetAddress &operator=(const InetAddress& address)
  {
    this->is_v4_ = address.is_v4_;
    addr6_ = address.addr6_;
    return *this;
  }

  bool v4() const { return is_v4_; }

  bool v6() const { return !is_v4_; }

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
  InetAddress() : is_v4_(true) { memset(&addr_, 0, sizeof(addr_)); }

private:
  bool is_v4_;

  union
  {
    struct in_addr addr_;
    struct in6_addr addr6_;
  };

  friend class InetSocketAddress;

};

}

#endif //LIBNET_DISTRIBUTION_INETADDRESS_H
