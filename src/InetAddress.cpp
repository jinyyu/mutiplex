#include "InetAddress.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Exception.h"
#include "Logger.h"
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





}
