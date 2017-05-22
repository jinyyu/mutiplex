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

TcpServer::TcpServer(int port, int num_io_threads)
    : port_(port),
      num_io_threads_(num_io_threads) ,
      state_(CREATE),
      index_(0),
      io_loops_(num_io_threads, nullptr)
{
}

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
  if (state_ != CREATE) {
    LOG_ERROR("error state %d", state_);
    return;
  }

  for (int i = 0; i < num_io_threads_; ++i) {
    auto run = [i, this] {
      EventLoop* loop = new EventLoop();
      loop->connection_established_callback(connection_established_callback_);
      loop->read_message_callback(read_message_callback_);
      loop->connection_closed_callback(connection_closed_callback_);

      loop->allocate_receive_buffer(6 * 1024 * 1024); //6M

      io_loops_[i] = loop;
      loop->run();
    };
    std::thread thread(run);
    thread.detach();
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


}