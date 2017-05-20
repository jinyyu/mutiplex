#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"

#include <unistd.h>

namespace net
{


Connection::Connection(int fd,
                       EventLoop* loop,
                       const InetSocketAddress& local,
                       const InetSocketAddress& peer)
    : channel_(nullptr),
      fd_(fd),
      peer_(peer),
      local_(local),
      loop_(loop),
      state_(NEW)
{

}

Connection::~Connection()
{
  if (channel_)
    delete(channel_);
  ::close(fd_);
}


void Connection::accept()
{
  if (state_ != NEW) {
    LOG_ERROR("state = %d", state_);
  }
  channel_ = new Channel(loop_->selector(), fd_);

}

}
