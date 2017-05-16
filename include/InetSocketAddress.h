#ifndef LIBNET_DISTRIBUTION_INETSOCKETADDRESS_H
#define LIBNET_DISTRIBUTION_INETSOCKETADDRESS_H
#include <InetAddress.h>

namespace net
{

class InetSocketAddress
{
public:

private:
  InetSocketAddress() {}

  int port_;
  InetAddress addr_;
};

}



#endif //LIBNET_DISTRIBUTION_INETSOCKETADDRESS_H
