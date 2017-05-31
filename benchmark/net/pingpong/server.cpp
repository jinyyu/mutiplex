#include <TcpServer.h>
#include <CircularBuffer.h>
#include <Connection.h>
#include <ByteBuffer.h>
#include <Logger.h>

using namespace net;

#define NUM_THREADS 2

class EchoServer
{
public:
  EchoServer(int port)
      : server_(port, NUM_THREADS)
  {
    ReadMessageCallback cb = [this](ConnectionPtr conn, ByteBuffer* buf, const Timestamp& timestamp)
    {
      this->hande_read(conn, buf, timestamp);
    };

    server_.read_message_callback(cb);
  }

  void run()
  {
    server_.run();
  }

private:
  void hande_read(ConnectionPtr conn, ByteBuffer* buf, const Timestamp &)
  {
    conn->write(buf->data(), buf->remaining());
  }

  TcpServer server_;
};

int main(int argc, char* argv[])
{
  if (argc != 2){
    printf("usage %s <port>\n", argv[0]);
    return -1;
  }
  //set_log_level(Logger::ERROR);
  
  int port = atoi(argv[1]);

  EchoServer server(port);
  server.run();
}