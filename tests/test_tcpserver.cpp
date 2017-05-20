#include <TcpServer.h>
#include <Logger.h>
using namespace net;

#include <callbacks.h>


bool connection_com(ConnectionPtr, const Timestamp & timestamp) {
  return true;
}

void closed(ConnectionPtr, const Timestamp& timestamp) {
  LOG_INFO("connection closed");
}

int main(int argc, char* argv[])
{
  TcpServer tcpServer(2048, 2);
  tcpServer.connection_established_callback(connection_com);
  tcpServer.connection_closed_callback(closed);
  tcpServer.run();

}
