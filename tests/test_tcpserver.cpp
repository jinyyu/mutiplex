#include <TcpServer.h>
#include <Logger.h>
using namespace net;

int main(int argc, char* argv[])
{
  TcpServer tcpServer(2048, 2);
  tcpServer.run();

}
