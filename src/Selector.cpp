#include "Selector.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <memory.h>
#include "Logger.h"
#include "Channel.h"


namespace net
{

Selector::Selector()
{
  epoll_fd_ =  epoll_create1(EPOLL_CLOEXEC);
  if (epoll_fd_ == -1) {
    LOG("epoll_create1 error %d", errno);
  }
}


Selector::~Selector()
{
  ::close(epoll_fd_);
}


void Selector::add(const SelectionKey& selection_key)
{
  control(EPOLL_CTL_ADD, selection_key);
}

void Selector::modify(const SelectionKey& selection_key)
{
  control(EPOLL_CTL_MOD, selection_key);
}

void Selector::remove(const SelectionKey& selection_key)
{
  control(EPOLL_CTL_DEL, selection_key);
}

void Selector::control(int op, const SelectionKey& selection_key)
{
  Channel* channel = selection_key.channel();
  struct epoll_event event;
  bzero(&event, sizeof(event));
  event.data.ptr = channel;
  if (epoll_ctl(epoll_fd_, op, channel->fd(), &event) != 0) {
    LOG("epoll_ctl error, op = %d, errno = %d", op, errno);
  }
}







}