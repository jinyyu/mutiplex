#include "InetAddress.h"
#include "Exception.h"

#include <arpa/inet.h>

namespace net
{

InetAddress InetAddress::get_by_address(const char* addr, Family family)
{
  InetAddress address;
  switch (family){
    case INET:
      address.is_v4_ = true;
      break;
    case INET6:
      address.is_v4_ = false;
      break;
    default:
      throw Exception::not_supported("invalid address familie");
  }
  int af = address.is_v4_ ? AF_INET : AF_INET6;

  if (inet_pton(af, addr, address.addr_) != 1) {
    throw Exception::invalid_argument("invalid address");
  }
  return address;
}

InetAddress InetAddress::any( Family family)
{
  InetAddress address;
  switch (family) {
    case INET: {
      address.is_v4_ = true;
      int any = htonl(INADDR_ANY);
      memcpy(address.addr_, &any, sizeof(any));
      break;
    }
    case INET6: {
      address.is_v4_ = false;
      in6_addr any = in6addr_any;
      memcpy(address.addr_, &any, sizeof(any));
      break;
    }
    default:
      throw Exception::not_supported("invalid address familie");
  }

  return address;
}


}
