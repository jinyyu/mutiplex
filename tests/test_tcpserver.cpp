#include <TcpServer.h>
#include <Logger.h>
#include <ByteBuffer.h>
#include <Connection.h>
#include <Timestamp.h>

using namespace net;

void connection_com(ConnectionPtr conn, const Timestamp & timestamp) {
  LOG_INFO("new connection time = %s", timestamp.to_string().c_str());
}

void closed(ConnectionPtr conn, const Timestamp& timestamp) {
  LOG_INFO("connection closed   = %s", timestamp.to_string().c_str());
}

void read_cb(ConnectionPtr conn, ByteBuffer* buf, const Timestamp &)
{

  conn->write(buf->data(), static_cast<uint32_t>(buf->remaining()));
  conn->set_default_timeout();

}

int main(int argc, char* argv[])
{
  TcpServer tcpServer(2048, 2);
  tcpServer.connection_established_callback(connection_com);
  tcpServer.connection_closed_callback(closed);
  tcpServer.read_message_callback(read_cb);
  tcpServer.run();

}
