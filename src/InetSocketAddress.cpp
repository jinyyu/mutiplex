#include "Status.h"
#include "InetSocketAddress.h"
#include "Exception.h"
#include "Logger.h"


namespace net
{

InetSocketAddress::InetSocketAddress(const InetAddress& addr, int port)
{
  if(addr.is_v4_) {
    sockaddr_.sin_port = htons(port);
    sockaddr_.sin_family = AF_INET;
    sockaddr_.sin_addr = addr.addr_;

  } else {
    sockaddr6_.sin6_family = AF_INET6;
    sockaddr6_.sin6_port = htons(port);
    sockaddr6_.sin6_addr = addr.addr6_;
  }
}

InetSocketAddress::InetSocketAddress(int port)
    : InetSocketAddress(InetAddress::any(InetAddress::INET), port)
{
}

InetSocketAddress::InetSocketAddress(const char* hostname, int port)
{
  Status s;
  InetAddress addr = InetAddress::get_by_host(hostname, s);
  if (!s.is_ok()) {
    throw Exception::invalid_argument("invalid hostname");
  }

  if(addr.is_v4_) {
    sockaddr_.sin_port = htons(port);
    sockaddr_.sin_family = AF_INET;
    sockaddr_.sin_addr = addr.addr_;

  } else {
    sockaddr6_.sin6_family = AF_INET6;
    sockaddr6_.sin6_port = htons(port);
    sockaddr6_.sin6_addr = addr.addr6_;
  }
}

int InetSocketAddress::get_port() const
{
  sockaddr* p =(sockaddr*) &(sockaddr_);
  if (p->sa_family == AF_INET) {
    return ntohs(sockaddr_.sin_port);
  }
  else {
    return ntohs(sockaddr6_.sin6_port);
  }
}


InetAddress InetSocketAddress::get_address() const
{
  InetAddress addr;

  sockaddr* p =(sockaddr*) &(sockaddr_);
  if (p->sa_family == AF_INET) {
    addr.addr_ = sockaddr_.sin_addr;
  }
  else {
    addr.addr6_ = sockaddr6_.sin6_addr;
  }


  return addr;
}

}