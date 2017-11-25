#include "net4cxx/TcpServer.h"
#include "net4cxx/EventLoop.h"
#include "net4cxx/Acceptor.h"
#include "net4cxx/Connection.h"
#include "net4cxx/Timestamp.h"
#include "net4cxx/InetAddress.h"
#include "net4cxx/InetSocketAddress.h"
namespace net4cxx
{

TcpServer::TcpServer(int port, int num_io_threads)
    : port_(port),
      num_io_threads_(num_io_threads)
{
}

TcpServer::~TcpServer()
{
    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        delete (*it);
    }
}

void TcpServer::run()
{
    io_loops_.resize(num_io_threads_, NULL);
    auto run_cb = [this](int index)
    {
        EventLoop *loop = new EventLoop();
        loop->allocate_receive_buffer(6 * 1024 * 1024); //6M
        loop->enable_timing_wheel(30);
        NewConnectionCallback cb = [this, loop](int fd,
                                                const Timestamp &timestamp,
                                                const InetSocketAddress &local,
                                                const InetSocketAddress &peer)
        {
            ConnectionPtr conn(new Connection(fd, loop, local, peer));
            conn->connection_established_callback(connection_established_callback_);
            conn->read_message_callback(read_message_callback_);
            conn->connection_closed_callback(connection_closed_callback_);
            loop->on_new_connection(conn, timestamp);
        };
        Acceptor acceptor(loop, port_);
        acceptor.new_connection_callback(cb);
        io_loops_[index] = loop;
        loop->run();
    };

    for (int i = 1; i < num_io_threads_; ++i) {
        std::thread thread(std::bind(run_cb, i));
        thread.detach();
    }

    run_cb(0);
}

void TcpServer::shutdown()
{
    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        (*it)->stop();
    }
}

}