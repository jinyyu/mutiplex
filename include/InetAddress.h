#ifndef LIBNET_DISTRIBUTION_INETADDRESS_H
#define LIBNET_DISTRIBUTION_INETADDRESS_H
#include <stdlib.h>
#include <string.h>

namespace net
{

class InetAddress
{

 public:
  bool v4() const { return is_v4_; }

  bool v6() const { return !is_v4_; }

  InetAddress(const InetAddress& address)
  {
    this->is_v4_ = address.is_v4_;
    memcpy(addr_, address.addr_, 46);
  }

  InetAddress& operator= (const InetAddress& address)
  {
    this->is_v4_ = address.is_v4_;
    memcpy(addr_, address.addr_, 46);
    return *this;
  }

  enum Family
  {
    INET = 1,
    INET6 = 2
  };

  //No name service is checked for the validity of the address
  static InetAddress get_by_address(const char* addr, Family family);

  //Determines the IP address of a host, given the host's name
  //static InetAddress get_by_name(const char* addr);

 private:
  InetAddress() {}

 private:
  bool is_v4_;
  char addr_[46];

};

}

#endif //LIBNET_DISTRIBUTION_INETADDRESS_H
