#ifndef NET4CXX_DISTRIBUTION_TCPSERVER_H
#define NET4CXX_DISTRIBUTION_TCPSERVER_H
#include <net4cxx/NonCopyable.h>
#include <net4cxx/callbacks.h>
#include <vector>
#include <thread>
#include <memory>

namespace net4cxx
{
class EventLoop;
class Timestamp;
class InetSocketAddress;

class TcpServer: NonCopyable
{
public:
    explicit TcpServer(int port, int num_io_threads);

    ~TcpServer();

    void connection_established_callback(const ConnectionEstablishedCallback& cb)
    { connection_established_callback_ = cb; }

    void read_message_callback(const ReadMessageCallback& cb)
    { read_message_callback_ = cb; }

    void connection_closed_callback(const ConnectionClosedCallback& cb)
    { connection_closed_callback_ = cb; }

    void run();

    void shutdown();
private:
    int port_;
    int num_io_threads_;
    std::vector<EventLoop*> io_loops_;

    ConnectionEstablishedCallback connection_established_callback_;
    ReadMessageCallback read_message_callback_;
    ConnectionClosedCallback connection_closed_callback_;

};

}


#endif //LIBNET_DISTRIBUTION_TCPSERVER_H
