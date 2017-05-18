#include "SelectionKey.h"
#include "Channel.h"

#include <sys/epoll.h>

namespace net
{

const uint32_t SelectionKey::OP_IN = EPOLLIN; //There is data to read

const uint32_t SelectionKey::OP_OUT = EPOLLOUT; //There is data to read

const uint32_t SelectionKey::OP_PRI = EPOLLPRI; //There is urgent data to read

const uint32_t SelectionKey::OP_ERR = EPOLLERR; //Error condition

const uint32_t SelectionKey::OP_HUP =  EPOLLHUP; //Hung up (the connection has been broken, usually for pipes and sockets)

int SelectionKey::fd()
{
  return channel_->fd();
}


Selector* SelectionKey::selector() const
{
  return channel_->selector_;
}


}