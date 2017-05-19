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
  TcpServer(int port, int num_io_threads)
      : port_(port),
        num_io_threads_(num_io_threads) ,
        state_(CREATE),
        index_(0)
  { }

  ~TcpServer();


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


  int port_;
  int num_io_threads_;
  EventLoop* accept_loop_;
  Acceptor* acceptor_;
  std::vector<EventLoop*> io_loops_;
  int index_;

  enum {
    CREATE = 0,
    RUNNING = 1,
    SHUTDOWN = 2
  };

  int state_;





};


}


#endif //LIBNET_DISTRIBUTION_TCPSERVER_H
