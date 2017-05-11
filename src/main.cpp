#include "Logger.h"

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
  using namespace net;
  set_log_destination("/tmp/bbb.txt");
  LOG("AAA");

}