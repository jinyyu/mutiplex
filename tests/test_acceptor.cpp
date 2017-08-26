#include <libnet/Logger.h>
#include <libnet/EventLoop.h>
#include <libnet/Acceptor.h>
#include <unistd.h>
#include <libnet/Timestamp.h>
#include <libnet/InetSocketAddress.h>

using namespace net;

void cb(int fd, const Timestamp &timestamp, const InetSocketAddress &local, const InetSocketAddress &peer)
{

    const char *str = "goodby\n";
    ::write(fd, str, strlen(str));
    LOG_INFO("[%s] local = %s, peer = %s",
             timestamp.to_string().c_str(),
             local.to_string().c_str(),
             peer.to_string().c_str());
    ::close(fd);
}

int main(int argc, char *argv[])
{
    EventLoop loop;
    Acceptor acceptor(&loop, 2048);
    acceptor.new_connection_callback(cb);
    loop.run();

    // telnet localhost 2048
}
