#include <TcpServer.h>
#include <Logger.h>
#include <ByteBuffer.h>
#include <Connection.h>

using namespace net;


bool connection_com(ConnectionPtr, const Timestamp & timestamp) {
  return true;
}

void closed(ConnectionPtr, const Timestamp& timestamp) {
  LOG_INFO("connection closed");
}

void read_cb(ConnectionPtr conn, ByteBuffer* buf, const Timestamp &)
{

  conn->write(buf->data(), static_cast<uint32_t>(buf->remaining()));

}

int main(int argc, char* argv[])
{
  TcpServer tcpServer(2048, 2);
  tcpServer.connection_established_callback(connection_com);
  tcpServer.connection_closed_callback(closed);
  tcpServer.read_message_callback(read_cb);
  tcpServer.run();

}
