#include "libnet/TcpServer.h"
#include "libnet/EventLoop.h"
#include "libnet/Acceptor.h"
#include "libnet/Logger.h"
#include "libnet/Connection.h"
#include "libnet/Timestamp.h"
#include "libnet/InetAddress.h"
#include "libnet/InetSocketAddress.h"

#include <unistd.h>


namespace net
{

TcpServer::TcpServer(int port, int num_io_threads)
    : port_(port),
      num_io_threads_(num_io_threads),
      index_(0),
      accept_loop_(nullptr)
{
}

TcpServer::~TcpServer()
{
    if (accept_loop_) delete (accept_loop_);

    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        delete (*it);
    }
}

void TcpServer::run()
{
    for(int i = 0; i < num_io_threads_; ++i) {
        EventLoop* loop = new EventLoop();
        loop->allocate_receive_buffer(6 * 1024 * 1024); //6M
        loop->enable_timing_wheel(30);
        io_loops_.push_back(loop);
    }

    for (int i = 0; i < num_io_threads_; ++i) {
        auto run = [i, this]
        {
            io_loops_[i]->run();
        };
        std::thread thread(run);
        thread.detach();
    }

    accept_loop_ = new EventLoop();
    Acceptor acceptor(accept_loop_, port_);

    NewConnectionCallback cb = [this](int fd,
                                      const Timestamp &timestamp,
                                      const InetSocketAddress &local,
                                      const InetSocketAddress &peer)
    {
        EventLoop *loop = next_loop();
        ConnectionPtr conn(new Connection(fd, loop, local, peer));
        conn->connection_established_callback(connection_established_callback_);
        conn->read_message_callback(read_message_callback_);
        conn->connection_closed_callback(connection_closed_callback_);

        loop->on_new_connection(conn, timestamp);
    };

    acceptor.new_connection_callback(cb);
    accept_loop_->run();
}

void TcpServer::shutdown()
{
    accept_loop_->stop();
    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        (*it)->stop();
    }
}

}