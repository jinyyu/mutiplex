#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"

#include <unistd.h>

namespace net
{


Connection::Connection(int fd,
                       EventLoop* loop,
                       const InetSocketAddress& local,
                       const InetSocketAddress& peer)
    : channel_(new Channel(loop->selector(), fd)),
      fd_(fd),
      peer_(peer),
      local_(local),
      loop_(loop)
{

}


Connection::~Connection()
{
  delete(channel_);
  ::close(fd_);
}

}
