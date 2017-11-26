#include <net4cxx/reactor/EventLoop.h>
#include <net4cxx/reactor/Acceptor.h>
#include <unistd.h>
#include <net4cxx/common/Timestamp.h>
#include <net4cxx/common/InetSocketAddress.h>

#include <log4cxx/logger.h>

using namespace net4cxx;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

void cb(int fd, const Timestamp &timestamp, const InetSocketAddress &local, const InetSocketAddress &peer)
{

    const char *str = "goodby\n";
    ::write(fd, str, strlen(str));
    LOG4CXX_INFO(logger,timestamp.to_string() << " " << local.to_string() << " " << peer.to_string());
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
