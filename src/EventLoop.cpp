#include "EventLoop.h"
#include "utils.h"
#include "Selector.h"
#include "Channel.h"
#include "SelectionKey.h"

namespace net
{

EventLoop::EventLoop()
    : pthread_id_(pthread_self()),
      is_quit_(false),
      selector_(new Selector(pthread_id_)),
      active_keys_(128)
{
  pthread_mutex_init(&mutex_, NULL);
  active_keys_.reserve(128);
}


EventLoop::~EventLoop()
{
  pthread_mutex_destroy(&mutex_);
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

}