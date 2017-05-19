#ifndef LIBNET_DISTRIBUTION_CONNECTION_H
#define LIBNET_DISTRIBUTION_CONNECTION_H
#include "NonCopyable.h"
#include <memory>

#include "InetSocketAddress.h"
#include "InetAddress.h"

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

  const InetSocketAddress& local_socket_address() const { return local_; }

  const InetSocketAddress& peer_socket_address() const { return peer_; }

  InetAddress local_address() const { return local_.get_address(); }

  InetAddress peer_address() const { return peer_.get_address(); }

  int local_port() const { return local_.port(); }

  int peer_port() const { return peer_.port(); }

  int fd() const { return fd_; }

private:
  int fd_;
  EventLoop* loop_;
  Channel* channel_;
  InetSocketAddress local_;
  InetSocketAddress peer_;

};




}

#endif //LIBNET_DISTRIBUTION_CONNECTION_H