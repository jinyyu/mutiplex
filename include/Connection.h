#ifndef LIBNET_DISTRIBUTION_CONNECTION_H
#define LIBNET_DISTRIBUTION_CONNECTION_H
#include "NonCopyable.h"
#include <memory>

#include "InetSocketAddress.h"
#include "InetAddress.h"
#include "callbacks.h"
#include "ByteBuffer.h"

#include <deque>

namespace net
{
class EventLoop;
class Selector;
class Channel;

class Connection : public std::enable_shared_from_this<Connection>, NonCopyable
{
public:
  explicit Connection(int fd,
                      EventLoop* loop,
                      const InetSocketAddress& local,
                      const InetSocketAddress& peer);

  ~Connection();

  void set_buffer_size(uint32_t size) { buffer_size_ = size; }

  const InetSocketAddress& local_socket_address() const { return local_; }

  const InetSocketAddress& peer_socket_address() const { return peer_; }

  InetAddress local_address() const { return local_.get_address(); }

  InetAddress peer_address() const { return peer_.get_address(); }

  int local_port() const { return local_.port(); }

  int peer_port() const { return peer_.port(); }

  int fd() const { return fd_; }

  void read_message_callback(const ReadMessageCallback& cb) {read_message_callback_ = cb; }

  void connection_closed_callback(const ConnectionClosedCallback& cb) { connection_closed_callback_ = cb; }

  void error_callback(const ErrorCallback& cb) {error_callback_ = cb; }

  void close();

  bool is_closed() { return state_ == Closed || state_ == Disconnecting; }

  bool write(void* data, int len);

private:
  friend class EventLoop;
  void accept();

  void handle_write(const Timestamp &timestamp);



private:
  enum State
  {
    New = 0,
    Receiving = 1,
    Disconnecting =  2,
    Closed = 3

  };

  int fd_;
  uint8_t state_;
  uint32_t buffer_size_;
  EventLoop* loop_;
  Channel* channel_;
  InetSocketAddress local_;
  InetSocketAddress peer_;

  ReadMessageCallback read_message_callback_;
  ConnectionClosedCallback connection_closed_callback_;
  ErrorCallback error_callback_;
  ByteBufferPtr buffer_in_;

  std::deque<ByteBufferPtr> buffer_out_;
  int buffer_out_remaining_;

};

}

#endif //LIBNET_DISTRIBUTION_CONNECTION_H
