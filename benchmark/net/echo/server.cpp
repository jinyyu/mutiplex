#include <TcpServer.h>
#include <CircularBuffer.h>


void read_cb(ConnectionPtr conn, CircularBuffer* buf, const Timestamp &)
{

}

int main(int argc, char* argv[])
{
  if (argc != 2){
    printf("usage %s <port>\n", argv[1]);
    return -1;
  }


}