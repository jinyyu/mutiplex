#ifndef LIBNET_DISTRIBUTION_TCPSERVER_H
#define LIBNET_DISTRIBUTION_TCPSERVER_H
#include "NonCopyable.h"
#include "callbacks.h"
#include <vector>

namespace net
{
class Acceptor;
class EventLoop;
class Timestamp;
class InetSocketAddress;


class TcpServer : NonCopyable
{
public:
  TcpServer(int port, int num_io_threads) : port_(port), num_io_threads_(num_io_threads), is_run_(false) { }

  ~TcpServer();

  void set_new_connection_callback(const NewConnectionCallback& cb) {
    user_new_connection_callback_ = cb;
  }

  void run();

  void shutdown();

private:
  EventLoop* next_loop()
  {
    if (index_ == io_loops_.size()) {
      index_ = 0;
    }
    return io_loops_[index_++];
  }

  void on_new_connection(int fd, const Timestamp& timestamp, const InetSocketAddress& address);

  int port_;
  int num_io_threads_;
  EventLoop* accept_loop_;
  Acceptor* acceptor_;
  std::vector<EventLoop*> io_loops_;
  int index_;
  bool is_run_;

  NewConnectionCallback user_new_connection_callback_;




};


}


#endif //LIBNET_DISTRIBUTION_TCPSERVER_H
