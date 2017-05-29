#include "ClientSocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "InetSocketAddress.h"
#include "Logger.h"

namespace net
{


ClientSocket::ClientSocket(int family)
    :fd_(socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP))
{
  //fix me ipv6
  if (fd_ < 0) {
    LOG_ERROR("create socket error %d", errno);
  }
}


ClientSocket::~ClientSocket()
{
  ::close(fd_);
}


bool ClientSocket::connect(const InetSocketAddress& addr)
{
  int len = addr.family() == AF_INET ? sizeof(addr.sockaddr_) : sizeof(addr.sockaddr6_);
  int ret = ::connect(fd_, addr.sockaddr_cast(), len);
  int err = errno;

  if (ret < 0 && err != EINPROGRESS) {
    LOG_ERROR("connect error %d", errno);
    return false;
  }
  return true;
}



}
