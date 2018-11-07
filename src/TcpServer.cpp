#include "mutiplex/TcpServer.h"
#include "mutiplex/Acceptor.h"
#include "mutiplex/Connection.h"
#include "mutiplex/Timestamp.h"
#include "Debug.h"

namespace muti
{


TcpServer::TcpServer(const std::string& addr, int num_io_threads)
    : addr_(addr),
      num_io_threads_(num_io_threads)
{
}

TcpServer::~TcpServer()
{
    for (auto i = 0; i < threads_.size(); ++i) {
        threads_[i].join();
    }
    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        delete (*it);
    }
}

void TcpServer::run()
{
    io_loops_.resize(num_io_threads_, NULL);
    auto run_cb = [this](int index) {
        EventLoop* loop = new EventLoop();
        loop->allocate_receive_buffer(64 * 1024);
        NewConnectionCallback cb = [this, loop](int fd,
                                                uint64_t timestamp,
                                                const InetAddress& local,
                                                const InetAddress& peer) {
            ConnectionPtr conn(new Connection(fd, loop, local, peer));
            conn->set_established_callback(established_callback_);
            conn->set_read_callback(read_callback_);
            conn->set_closed_callback(closed_callback_);
            loop->on_new_connection(conn, timestamp);
        };
        Acceptor acceptor(loop, addr_);
        acceptor.new_connection_callback(cb);
        io_loops_[index] = loop;
        loop->run();
        LOG_DEBUG("loop exit %d", index);

        std::unique_lock<std::mutex> lock(mutex_);
        --count_down_;
        cv_.notify_one();
    };

    count_down_ = num_io_threads_;
    for (int i = 0; i < num_io_threads_; ++i) {
        threads_.emplace_back(std::bind(run_cb, i));
    }
    std::unique_lock<std::mutex> lock(mutex_);
    while (count_down_ > 0) {
        cv_.wait(lock);
    }
}

void TcpServer::shutdown()
{
    for (auto it = io_loops_.begin(); it < io_loops_.end(); ++it) {
        (*it)->stop();
    }
}

}