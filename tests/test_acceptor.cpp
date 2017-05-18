#include <Logger.h>
#include <EventLoop.h>
#include <Acceptor.h>
#include <unistd.h>
#include <Timestamp.h>
#include <InetSocketAddress.h>

using namespace net;

void cb(int fd, const Timestamp& timestamp, const InetSocketAddress& address)
{

  const char* str = "goodby\n";
  ::write(fd, str, strlen(str));
  LOG_INFO("time = %s, addr =%s, port =%d", timestamp.to_string().c_str(), address.get_address().to_string().c_str(), address.port());
  ::close(fd);
}


int main(int argc, char* argv[])
{
  EventLoop loop;
  Acceptor acceptor(&loop, 2048);
  acceptor.new_connection_callback(cb);
  loop.run();

  // telnet localhost 2048
}
