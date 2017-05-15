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

  static InetAddress get_by_address(const char* addr, bool is_v4 = true);

 private:
  InetAddress(bool is_v4) : is_v4_(is_v4) {}

 private:
  bool is_v4_;
  char addr_[46];

};


}

#endif //LIBNET_DISTRIBUTION_INETADDRESS_H
