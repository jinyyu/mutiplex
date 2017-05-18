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

  if (inet_pton(af, addr, &address.addr6_) != 1) {
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
      address.addr6_ = in6addr_any;
      break;
    }
    default:
      throw Exception::not_supported("invalid address familie");
  }

  return address;
}


InetAddress InetAddress::get_by_host(const char* hostname, Status& status)
{
  InetAddress addr;
  char buf[8192];
  struct hostent hent;
  struct hostent* he = NULL;
  int herrno = 0;
  bzero(&hent, sizeof(hent));

  //only for ip v4
  addr.is_v4_ = true;
  int ret = gethostbyname_r(hostname, &hent, buf, sizeof(buf), &he, &herrno);
  if (ret == 0 && he != NULL)  {
    addr.is_v4_ = true;
    memcpy(&addr.addr_, he->h_addr, sizeof(addr.addr_));
    status = Status::ok();
  }
  else {
    status = Status::invalid_argument("invalid host name");
  }
  return addr;
}


std::string InetAddress::to_string() const
{
  char str[INET6_ADDRSTRLEN];
  int af = v4() ? AF_INET : AF_INET6;

  if (inet_ntop(af, &addr_, str, INET6_ADDRSTRLEN) == NULL) {
    LOG_ERROR("inet_ntop error : %d", errno)
    throw Exception::invalid_argument("inet_ntop error");
  }
  return str;
}

bool InetAddress::operator== (const InetAddress& address)
{
  if (this->is_v4_ != address.is_v4_) {
    return false;
  }

  if (address.is_v4_) {
    return memcmp(&addr_, &address.addr_, sizeof(addr_)) == 0;
  }
  else {
    return memcmp(&addr6_, &address.addr6_, sizeof(addr6_)) == 0;
  }
}


bool InetAddress::operator!= (const InetAddress& address)
{
  return !InetAddress::operator==(address);
}

}
