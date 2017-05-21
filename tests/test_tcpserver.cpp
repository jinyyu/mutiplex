#include <TcpServer.h>
#include <Logger.h>
#include <ByteBuffer.h>
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

void read_cb(ConnectionPtr conn, ByteBufferPtr buf, const Timestamp &)
{
  int len = buf->remaining();
  char* buffer = (char*)malloc(len + 1);
  buffer[len] = '\0';

  buf->get(buffer, len);

  conn->write(buffer, len);
  LOG_INFO("%s", buffer);

  conn->write(buffer, len);
  LOG_INFO("%s", buffer);


  free(buffer);

}

int main(int argc, char* argv[])
{
  TcpServer tcpServer(2048, 2);
  tcpServer.connection_established_callback(connection_com);
  tcpServer.connection_closed_callback(closed);
  tcpServer.read_message_callback(read_cb);
  tcpServer.run();

}
