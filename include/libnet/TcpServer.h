#ifndef LIBNET_DISTRIBUTION_TCPSERVER_H
#define LIBNET_DISTRIBUTION_TCPSERVER_H
#include <libnet/NonCopyable.h>
#include <libnet/callbacks.h>
#include <vector>
#include <thread>
#include <memory>

namespace net
{
class EventLoop;
class Timestamp;
class InetSocketAddress;

class TcpServer: NonCopyable
{
public:
    explicit TcpServer(int port, int num_io_threads);

    ~TcpServer();

    void connection_established_callback(const ConnectionEstablishedCallback &cb)
    { connection_established_callback_ = cb; }

    void read_message_callback(const ReadMessageCallback &cb)
    { read_message_callback_ = cb; }

    void connection_closed_callback(const ConnectionClosedCallback &cb)
    { connection_closed_callback_ = cb; }

    void run();

    void shutdown();

private:
    EventLoop *next_loop()
    {
        if (index_ == io_loops_.size()) {
            index_ = 0;
        }
        return io_loops_[index_++];
    }

private:
    int port_;
    int num_io_threads_;
    EventLoop *accept_loop_;
    std::vector<EventLoop *> io_loops_;
    int index_;

    ConnectionEstablishedCallback connection_established_callback_;
    ReadMessageCallback read_message_callback_;
    ConnectionClosedCallback connection_closed_callback_;

};

}


#endif //LIBNET_DISTRIBUTION_TCPSERVER_H
