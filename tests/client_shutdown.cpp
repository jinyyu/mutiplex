#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <Logger.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace net;

int main(int argc, char* argv[])
{
  if (argc != 3) {
    printf("usage : %s : <ip> <port>", argv[0]);
    return -1;
  }
  struct in_addr addr;
  if (inet_pton(AF_INET, argv[1], &addr) != 1) {
    printf("invalid addr %s\n", argv[1]);
    return -1;
  }

  int port = atoi(argv[2]);

  int fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
  sockaddr_in socket_addr = {0,};
  socket_addr.sin_family = AF_INET;

  socket_addr.sin_addr = addr;
  socket_addr.sin_port = htons(port);


  if (0 !=::connect(fd, (sockaddr*)&socket_addr, sizeof(sockaddr_in))) {
    printf("connect error %d\n", errno);
    exit(-1);
  }

  int n = ::write(fd, "helo", 4);

  printf("write n = %d\n", n);

  shutdown(fd, SHUT_WR);
  printf("shutdown\n");
  sleep(5);

  close(fd);
  printf("closed\n");

}