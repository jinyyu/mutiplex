#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include "SelectionKey.h"
#include "ByteBuffer.h"
#include "CircularBuffer.h"

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
      buffer_out_(nullptr)
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

  if (buffer_out_) {
    delete buffer_out_;
  }

  LOG_INFO("connection closed");
}


void Connection::accept()
{
  if (state_ != New) {
    LOG_ERROR("state = %d", state_);
  }
  channel_ = new Channel(loop_->selector(), fd_);

  SelectionCallback read_cb = [this](const Timestamp & timestamp, SelectionKey * key)
  {
    ByteBuffer* buffer = loop_->recv_buffer_;
    buffer->clear();
    ssize_t n = ::read(fd_, buffer->data(), buffer->remaining());

    if (n < 0) {
      LOG_ERROR("read error %d", errno);
      if (error_callback_) {
        error_callback_(shared_from_this(), timestamp);
      }
    }
    else if (n == 0) {
      //peer shutdown read
      if (connection_closed_callback_) {
        connection_closed_callback_(shared_from_this(), timestamp);
      }
      channel_->disable_reading();

      close();

    } else {
      LOG_INFO("read n = %d", n);
      buffer->position(n);
      buffer->flip();

      if (read_message_callback_) {
        read_message_callback_(shared_from_this(), buffer, timestamp);
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

  if (!has_bytes_to_write()) {
    //closed it
    state_ = Closed;
    loop_->remove_connection(fd_);
  }
  else {
    //has bytes to write
    state_ = Disconnecting;
  }
}


bool Connection::write(const ByteBuffer& buffer)
{
  if (is_closed()) {
    return false;
  }
  if (loop_->is_in_loop_thread()) {
    do_write(buffer.data(), buffer.remaining());

  }
  else {
    ByteBuffer buff(buffer);
    auto callback = [this, buff] {
      this->do_write(buff.data(), buff.remaining());
    };
    loop_->post(callback);
  }
  return true;
}

bool Connection::write(const void* data, uint32_t len)
{
  if (is_closed()) {
    return false;
  }
  if (loop_->is_in_loop_thread()) {
    do_write(data, len);
  }
  else {
    ByteBuffer buffer(len);
    buffer.put(data, len);
    buffer.flip();

    //copy buffer
    auto callback = [this, buffer](){
      this->do_write(buffer.data(), buffer.remaining());
    };
    loop_->post(callback);
  }
  return true;
}

void Connection::do_write(const void* data, uint32_t len)
{
  if (!buffer_out_) {
    buffer_out_ = new CircularBuffer(len);
  }
  buffer_out_->put(data, len);
  channel_->enable_writing();
}


void Connection::handle_write(const Timestamp &timestamp)
{

  if (buffer_out_->empty()) {
    LOG_ERROR("buffer out is empty");
    exit(-1);
  }

  int total = buffer_out_->write_to_fd(this, timestamp);
  if (total < 0) {
    if (error_callback_)
      error_callback_(shared_from_this(), timestamp);
    state_ = Closed;
    channel_->disable_all();
    LOG_ERROR("error happened")
    close();
    return;
  }
  else if (total == 0) {
    //close
    state_ = Closed;
    channel_->disable_all();
    loop_->remove_connection(fd_);
    return;
  }
  //write success
  else if (state_ == Disconnecting) {
    channel_->disable_writing();
    close();
    return;
  }

  else if (buffer_out_->empty()) {
    channel_->disable_writing();
    return;
  }
}


bool Connection::has_bytes_to_write() const {
  return buffer_out_ && !buffer_out_->empty();
}

}
