#pragma once
#include <boost/noncopyable.hpp>
#include <evcpp/callbacks.h>
#include <vector>
#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>

namespace ev
{
class EventLoop;
class Timestamp;
class InetSocketAddress;

class TcpServer: boost::noncopyable
{
public:
    explicit TcpServer(int port, int num_io_threads);

    ~TcpServer();

    void connection_established_callback(const ConnectionEstablishedCallback& cb)
    {
        connection_established_callback_ = cb;
    }

    void read_message_callback(const ReadMessageCallback& cb)
    {
        read_message_callback_ = cb;
    }

    void connection_closed_callback(const ConnectionClosedCallback& cb)
    {
        connection_closed_callback_ = cb;
    }

    void run();

    void shutdown();
private:
    int port_;
    int num_io_threads_;
    std::vector<EventLoop*> io_loops_;
    std::vector<std::thread> threads_;
    ConnectionEstablishedCallback connection_established_callback_;
    ReadMessageCallback read_message_callback_;
    ConnectionClosedCallback connection_closed_callback_;

    std::mutex mutex_;
    std::condition_variable cv_;
    int count_down_;
};

}