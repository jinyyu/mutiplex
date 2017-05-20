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
      buffer_size_(1024),
      buffer_out_remaining_(0)
{

}

Connection::~Connection()
{
  if (channel_) {
    delete (channel_);
  }
  ::close(fd_);

  if (state_ != Closed) {
    LOG_ERROR("connection is now closed %d", state_);
  }
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
      close();
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

  SelectionCallback write_cb = [this](const Timestamp & timestamp, SelectionKey*) {
    this->handle_write(timestamp);
  };
  channel_->set_writing_selection_callback(write_cb);

  state_ = Receiving;
}

void Connection::close()
{
  if (buffer_out_.empty()) {
    state_ = Closed;
    loop_->remove_connection(fd_);

  } else {
    state_ = Disconnecting;
  }
}


bool Connection::write(void* data, int len)
{
  if (is_closed()) {
    return false;
  }
  ByteBufferPtr buf(new ByteBuffer(len));
  buf->put(data, len);
  buf->flip();

  buffer_out_.push_back(buf);

  channel_->enable_writing();

}


void Connection::handle_write(const Timestamp &timestamp)
{
  if (buffer_out_.empty()) {
    return;
  }
  std::vector<struct iovec> iovecs;

  for(auto it = buffer_out_.begin(); it != buffer_out_.end(); ++it) {
    ByteBufferPtr& buf = *it;
    struct iovec vec;
    vec.iov_base = buf->data();
    vec.iov_len = buf->remaining();
    iovecs.push_back(vec);
  }

  ssize_t total = writev(fd_, iovecs.data(), iovecs.size());

  if (total == -1) {
    LOG_ERROR("writev error %d", errno);
    return;
  }

  for(auto it = buffer_out_.begin(); it != buffer_out_.end(); ++it) {
    ByteBufferPtr& buf = *it;
    int remain = buf->remaining();
    if (remain > total) {
      buf->skip(total);
      break;
    }
    else if (remain == total) {
      buffer_out_.pop_front();
      break;
    }
    else {
      //remain < total
      buffer_out_.pop_front();
      total -= buf->remaining();
    }
  }

  if (state_ == Disconnecting && buffer_out_.empty()) {
    loop_->remove_connection(fd_);
  }
  else if (buffer_out_.empty()) {
    channel_->disable_writing();
  }
}

}
