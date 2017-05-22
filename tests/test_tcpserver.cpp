#include <TcpServer.h>
#include <Logger.h>
#include <CircularBuffer.h>
#include <Connection.h>
#include <string>

using namespace net;


#include <callbacks.h>


bool connection_com(ConnectionPtr, const Timestamp & timestamp) {
  return true;
}

void closed(ConnectionPtr, const Timestamp& timestamp) {
  LOG_INFO("connection closed");
}

void read_cb(ConnectionPtr conn, CircularBuffer* buf, const Timestamp &)
{
  char buffer[1024];
  uint32_t n = buf->get(buffer, 1024);
  buffer[n] = '\0';

  LOG_INFO("get n = %d, str = %s", n, buffer);

  conn->write(buffer, n);

}

int main(int argc, char* argv[])
{
  TcpServer tcpServer(2048, 2);
  tcpServer.connection_established_callback(connection_com);
  tcpServer.connection_closed_callback(closed);
  tcpServer.read_message_callback(read_cb);
  tcpServer.run();

}
