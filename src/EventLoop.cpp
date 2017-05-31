#include "EventLoop.h"
#include "utils.h"
#include "Selector.h"
#include "Channel.h"
#include "SelectionKey.h"
#include "Logger.h"
#include "InetSocketAddress.h"
#include "Connection.h"
#include "ByteBuffer.h"

#include <sys/eventfd.h>
#include <unistd.h>


namespace net
{

EventLoop::EventLoop()
    : pthread_id_(pthread_self()),
      is_quit_(false),
      selector_(new Selector(pthread_id_)),
      active_keys_(128),
      wakeup_fd_(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)),
      recv_buffer_(nullptr)
{
  pthread_mutex_init(&mutex_, NULL);

  active_keys_.reserve(128);

  if (wakeup_fd_ == -1) {
    LOG_ERROR("eventfd error %d", errno);
  }

  //setup wakeup channel
  wakeup_channel_ = new Channel(selector_, wakeup_fd_);
  wakeup_channel_->enable_reading([this](const Timestamp&, SelectionKey*) {
    uint64_t n;
    if (eventfd_read(wakeup_fd_, &n) < 0) {
      LOG_ERROR("eventfd_read error %d", errno);
    }
  });
}


EventLoop::~EventLoop()
{
  pthread_mutex_destroy(&mutex_);
  delete(wakeup_channel_);
  ::close(wakeup_fd_);

  connections_.clear();

  delete(selector_);

  if(recv_buffer_){
    delete(recv_buffer_);
  }

  LOG_INFO("[%lu] : loop exit", pthread_id_);

}


void EventLoop::run()
{
  if (!is_in_loop_thread()) {
    LOG_WARNING("not in loop thread");
  }

  while (!is_quit_) {
    active_keys_.clear();
    Timestamp time = selector_->select(8000, active_keys_);
    if (active_keys_.empty()) {
      //LOG_INFO("nothing happened")
      continue;
    }

    for(SelectionKey* key: active_keys_) {
      //LOG_INFO("fd = %d, op = %s", key->fd() ,SelectionKey::op_get_string(key->ready_ops()).c_str());
      Channel* channel = key->channel();
      if (key->is_error()) {
        channel->handle_error(time);
      }
      else {
        if (key->is_readable()) {
          channel->handle_read(time);
        }
        if (key->is_writable()) {
          channel->handle_wirte(time);
        }
      }
    }

    std::vector<Callback> callbacks;
    {
      MutexLockGuard lock(&mutex_);
      std::swap(callbacks, callbacks_);
    }
    for(int i = 0; i < callbacks.size(); ++i) {
      callbacks[i]();
    }
  }
}

void EventLoop::stop()
{
  is_quit_ = true;
}


void EventLoop::wake_up()
{
  if (is_in_loop_thread())
    return;

  if (eventfd_write(wakeup_fd_, 1) < 0) {
    LOG_ERROR("eventfd_write error %d", errno);
  }
}

void EventLoop::post(const Callback& callback)
{
  if (is_in_loop_thread()){
    callback();
  } else {
    {
      MutexLockGuard lock(&mutex_);
      callbacks_.push_back(callback);
    }
    wake_up();
  }
}


void EventLoop::on_new_connection(ConnectionPtr conn, const Timestamp& timestamp)
{
  Callback cb = [this, conn, timestamp]() {
    conn->accept();

    connections_[conn->fd()] = conn;

    if (conn->connection_established_callback_) {
      conn->connection_established_callback_(conn, timestamp);
    }
  };
  post(cb);

}

void EventLoop::allocate_receive_buffer(uint32_t capacity)
{
  recv_buffer_ = new ByteBuffer(capacity);
}


}