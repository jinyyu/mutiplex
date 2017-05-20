#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include "SelectionKey.h"

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
      state_(New),
      buffer_size_(1024)
{

}

Connection::~Connection()
{
  if (channel_) {
    delete (channel_);
  }
  ::close(fd_);
  LOG_INFO("connection closed");
}


void Connection::accept()
{
  if (state_ != New) {
    LOG_ERROR("state = %d", state_);
  }
  channel_ = new Channel(loop_->selector(), fd_);
  buffer_in_ = std::make_shared<ByteBuffer>(buffer_size_);

  SelectionCallback read_cb = [this](const Timestamp & timestamp, SelectionKey * key)
  {
    buffer_in_->clear();
    int n = ::read(fd_, buffer_in_->data(), buffer_in_->remaining());
    if (n < 0) {
      LOG_ERROR("read error %d", errno);
      if (error_callback_) {
        error_callback_(shared_from_this(), timestamp);
      }
    }
    else if (n == 0) {
      //closed
      if (connection_closed_callback_) {
        connection_closed_callback_(shared_from_this(), timestamp);
      }
      loop_->remove_connection(fd_);
    } else {
      LOG_INFO("read n = %d", n);
      buffer_in_->position(n);
      buffer_in_->flip();
      if (read_message_callback_) {
        read_message_callback_(shared_from_this(), buffer_in_, timestamp);
      }
    }
  };

  channel_->enable_reading(read_cb);
  state_ = Receiving;

}

void Connection::close()
{
  if (buffer_out_.empty()) {
    loop_->remove_connection(fd_);
  } else {
    state_ = Disconnecting;
  }
}

}
