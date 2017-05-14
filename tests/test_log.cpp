#include <Logger.h>

int main(int argc, char* argv[])
{
  using namespace net;
  set_log_destination("/tmp/test.txt");
  LOG("AAA");

  LOG("int %d", 999);
}