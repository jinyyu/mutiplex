#ifndef LIBNET_DISTRIBUTION_CLIENTSOCKET_H
#define LIBNET_DISTRIBUTION_CLIENTSOCKET_H
#include "NonCopyable.h"

namespace net {

class InetSocketAddress;

class ClientSocket : NonCopyable
{
public:
  explicit ClientSocket(int family);

  ~ClientSocket();

  int fd() const { return fd_; }

  bool connect(const InetSocketAddress& addr);

private:

  int fd_;
};

}

#endif //LIBNET_DISTRIBUTION_CLIENTSOCKET_H
