#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Logger.h"
#include <unistd.h>
#include <Timestamp.h>
#include <InetAddress.h>
#include <InetSocketAddress.h>


#include <thread>

namespace net
{


TcpServer::~TcpServer()
{

}

void TcpServer::run()
{
  using namespace std;
  is_run_ = true;

  accept_loop_ = new EventLoop();
  acceptor_ = new Acceptor(accept_loop_, port_);
  NewConnectionCallback cb = std::bind(&TcpServer::on_new_connection,
                                       this,
                                       placeholders::_1,
                                       placeholders::_2,
                                       placeholders::_3);

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



void TcpServer::on_new_connection(int fd, const Timestamp& timestamp, const InetSocketAddress& address)
{
  if (user_new_connection_callback_) {
    user_new_connection_callback_(fd, timestamp, address);
  }
  next_loop()->on_new_connection(fd);
}



}