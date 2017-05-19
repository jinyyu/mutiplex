#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Logger.h"
#include "Connection.h"
#include "Timestamp.h"
#include "InetAddress.h"
#include "InetSocketAddress.h"

#include <unistd.h>
#include <thread>

namespace net
{

TcpServer::~TcpServer()
{
  if (state_ == CREATE) {
    return;
  } else if (state_ == RUNNING) {
    shutdown();
  }

  delete(acceptor_);
  delete(accept_loop_);

  for (auto it = io_loops_.begin() ; it < io_loops_.end(); ++it) {
    delete(*it);
  }
}


void TcpServer::run()
{
  using namespace std;
  if (state_ != CREATE) {
    LOG_ERROR("error state %d", state_);
    return;
  }

  accept_loop_ = new EventLoop();
  acceptor_ = new Acceptor(accept_loop_, port_);

  NewConnectionCallback cb = [this](int fd,
                                    const Timestamp & timestamp,
                                    const InetSocketAddress& local,
                                    const InetSocketAddress& peer)
  {
    EventLoop* loop = next_loop();
    loop->on_new_connection(fd, timestamp, local, peer);
  };

  acceptor_->new_connection_callback(cb);

  for (int i = 0; i < num_io_threads_; ++i) {
    auto io_loop = [this, i]() {
      EventLoop* loop = new EventLoop();
      this->io_loops_.push_back(loop);
      loop->run();
    };

    thread io_thread(io_loop);
    io_thread.detach();
  }

  accept_loop_->run();
}



void TcpServer::shutdown()
{
  if (state_ != RUNNING) {
    return;
  }

  accept_loop_->stop();

  for (auto it = io_loops_.begin() ; it < io_loops_.end(); ++it) {
    (*it)->stop();
  }

}


void TcpServer::connection_established_callback(const ConnectionEstablishedCallback& cb)
{
  for (auto it = io_loops_.begin(); it != io_loops_.end(); ++it) {
    (*it)->connection_established_callback(cb);
  }
}



}