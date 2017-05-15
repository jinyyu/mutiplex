#include "InetAddress.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Exception.h"


namespace net
{


InetAddress InetAddress::get_by_address(const char* addr, bool is_v4)
{
  InetAddress address(is_v4);
  int af = is_v4 ? AF_INET : AF_INET6;

  if (inet_pton(af, addr, address.addr_) != 0) {
    throw Exception::invalid_argument("invalid address");
  }


}





}
