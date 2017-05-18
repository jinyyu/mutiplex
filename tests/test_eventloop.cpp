#include <EventLoop.h>
#include <thread>
#include <Logger.h>
#include <unistd.h>
using namespace net;



int main(int argc, char* argv[])
{

  EventLoop loop;

  EventLoop* p = &loop;

  std::thread thread([p]() {

    sleep(5);
    p->wake_up();

  });

  loop.run();
}