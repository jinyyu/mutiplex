#pragma once
#include <boost/noncopyable.hpp>
#include <evcpp/callbacks.h>
#include <vector>
#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <evcpp/EventLoop.h>
#include <evcpp/InetAddress.h>

namespace ev
{

class TcpServer: boost::noncopyable
{
public:
    explicit TcpServer(const std::string& addr, int num_io_threads);

    ~TcpServer();

    void set_established_callback(const EstablishedCallback& cb)
    {
        established_callback_ = cb;
    }

    void set_read_callback(const ReadCallback& cb)
    {
        read_callback_ = cb;
    }

    void connection_closed_callback(const ClosedCallback& cb)
    {
        closed_callback_ = cb;
    }

    void run();

    void shutdown();
private:
    InetAddress addr_;
    int num_io_threads_;
    std::vector<EventLoop*> io_loops_;
    std::vector<std::thread> threads_;
    EstablishedCallback established_callback_;
    ReadCallback read_callback_;
    ClosedCallback closed_callback_;

    std::mutex mutex_;
    std::condition_variable cv_;
    int count_down_;
};

}