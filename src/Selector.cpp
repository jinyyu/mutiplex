#include "Selector.h"
#include <unistd.h>
#include <memory.h>
#include "Logger.h"
#include "Channel.h"
#include "SelectionKey.h"
#include "Logger.h"


namespace net
{

Selector::Selector(pthread_t pthread_id)
    : events_(64),
      selecting_events_(0),
      pthread_id_(pthread_id)
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


void Selector::add(SelectionKey* selection_key)
{
  control(EPOLL_CTL_ADD, selection_key);
  ++selecting_events_;
}

void Selector::modify(SelectionKey*selection_key)
{
  control(EPOLL_CTL_MOD, selection_key);
}

void Selector::remove(SelectionKey* selection_key)
{
  control(EPOLL_CTL_DEL, selection_key);
  --selecting_events_;
}

void Selector::control(int op,SelectionKey* selection_key)
{
  struct epoll_event event;
  bzero(&event, sizeof(event));
  event.data.ptr = selection_key;
  event.events = selection_key->interest_ops();

  if (epoll_ctl(epoll_fd_, op, selection_key->fd(), &event) != 0) {
    LOG("epoll_ctl error, op = %d, errno = %d", op, errno);
  }
}


Timestamp Selector::select(int timeout_milliseconds, std::vector<SelectionKey*>& active_key)
{
  int n_events = epoll_wait(epoll_fd_, events_.data(), events_.size(), timeout_milliseconds);
  Timestamp cur = Timestamp::currentTime();
  if (n_events == -1) {
    LOG("epoll_wait error %d", errno);
  }
  else if (n_events == 0) {
    LOG("[%lu] selecting event = %d", pthread_id_, selecting_events_);
  }
  else {
    for (int i = 0; i < n_events; ++i) {
      SelectionKey *key = (SelectionKey *) events_[i].data.ptr;
      key->ready_ops(events_[i].events);
      active_key.push_back(key);
    }
  }

  return cur;
}







}