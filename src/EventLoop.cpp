#include "EventLoop.h"
#include "utils.h"
#include "Selector.h"
#include "Channel.h"
#include "SelectionKey.h"
#include "Logger.h"

#include <sys/eventfd.h>
#include <unistd.h>

namespace net
{

EventLoop::EventLoop()
    : pthread_id_(pthread_self()),
      is_quit_(false),
      selector_(new Selector(pthread_id_)),
      active_keys_(128),
      wakeup_fd_(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK))
{
  pthread_mutex_init(&mutex_, NULL);
  active_keys_.reserve(128);

  if (wakeup_fd_ == -1) {
    LOG("eventfd error %d", errno);
  }

  //setup wakeup channel
  wakeup_channel_ = new Channel(selector_, wakeup_fd_);
  wakeup_channel_->enable_reading([this](const Timestamp&, SelectionKey*) {
    uint64_t n;
    if (eventfd_read(wakeup_fd_, &n) < 0) {
      LOG("eventfd_read error %d", errno);
    }
  });
}


EventLoop::~EventLoop()
{
  pthread_mutex_destroy(&mutex_);
  delete(wakeup_channel_);
  ::close(wakeup_fd_);

  LOG("[%lu] : loop exit", pthread_id_);

}


void EventLoop::run()
{
  while (!is_quit_) {
    active_keys_.clear();
    Timestamp time = selector_->select(8000, active_keys_);
    for(SelectionKey* key: active_keys_) {
      Channel* channel = key->channel();
      if (key->is_readable()) {
        channel->handle_read(time);
      }
      if (key->is_writable()) {
        channel->handle_wirte(time);
      }
    }
  }
}


void EventLoop::wake_up()
{
  if (eventfd_write(wakeup_fd_, 1) < 0) {
    LOG("eventfd_write error %d", errno);
  }
}

}