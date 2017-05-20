#ifndef LIBNET_DISTRIBUTION_ACCEPTOR_H
#define LIBNET_DISTRIBUTION_ACCEPTOR_H

#include "NonCopyable.h"
#include "callbacks.h"

namespace net
{

class ServerSocket;
class Channel;
class Selector;
class EventLoop;
class InetSocketAddress;
class EventLoop;

class Acceptor : NonCopyable
{
public:
  Acceptor(EventLoop* loop, int port);

  Acceptor(EventLoop* loop,const InetSocketAddress& addr);

  ~Acceptor();

  void new_connection_callback(const NewConnectionCallback& callback) { callback_ = callback; }

private:
  EventLoop* loop_;
  ServerSocket* server_socket_;
  Channel* accept_channel_;
  InetSocketAddress* peer_addr_;
  InetSocketAddress* local_addr_;
  NewConnectionCallback callback_;

};


}


#endif //LIBNET_DISTRIBUTION_ACCEPTOR_H
