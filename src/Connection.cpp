#include "Connection.h"
#include "Channel.h"

#include <unistd.h>

namespace net
{


Connection::Connection(int fd,
                       Selector* selector,
                       const InetSocketAddress& local,
                       const InetSocketAddress& peer)
    : channel_(new Channel(selector, fd)),
      fd_(fd),
      peer_(peer),
      local_(local)
{

}


Connection::~Connection()
{
  delete(channel_);
  ::close(fd_);
}

}
