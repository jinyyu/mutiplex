#include "InetAddress.h"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

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

  if (inet_pton(af, addr, &address.addr_) != 1) {
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
      memcpy(&address.addr_, &any, sizeof(any));
      break;
    }
    case INET6: {
      address.is_v4_ = false;
      address.addr_ = in6addr_any;
      break;
    }
    default:
      throw Exception::not_supported("invalid address familie");
  }

  return address;
}


InetAddress InetAddress::get_by_host(const char* hostname, Status& status)
{
  struct addrinfo hints;
  struct addrinfo* res = nullptr;
  int ret;
  InetAddress addr;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  ret = getaddrinfo(hostname, NULL, &hints, &res);

  for(addrinfo* it = res; it != NULL; it = it->ai_next) {
    switch (it->ai_family) {
      case AF_INET: {
        addr.is_v4_ = true;
        struct sockaddr_in *in4 = (sockaddr_in *) &it->ai_addr;
        memcpy(&addr.addr_, &in4->sin_addr, sizeof(&in4->sin_addr));
        status = Status::ok();
        goto cleanup;
        break;
      }
      case AF_INET6: {
        addr.is_v4_ = false;
        struct sockaddr_in6 *in6 = (sockaddr_in6 *) &it->ai_addr;
        addr.addr_ = in6->sin6_addr;
        status = Status::ok();
        goto cleanup;
        break;
      }
      default:
        continue;
    }
  }

  status = Status::invalid_argument("invalid hostname");
cleanup:
  if (res) {
    freeaddrinfo(res);
  }
}


std::string InetAddress::to_string()
{
  char str[INET6_ADDRSTRLEN];
  int af = v4() ? AF_INET : AF_INET6;

  if (inet_ntop(af, &addr_, str, INET6_ADDRSTRLEN) == NULL) {
    LOG("inet_ntop error : %d", errno)
    throw Exception::invalid_argument("inet_ntop error");
  }
  return str;
}

}
