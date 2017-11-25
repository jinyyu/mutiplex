#include <net4cxx/Timer.h>
#include <net4cxx/EventLoop.h>
#include <net4cxx/Logger.h>
#include <net4cxx/Timestamp.h>

using namespace net4cxx;

int main(int argc, char *argv[])
{
    EventLoop loop;
    Timer timer(&loop);

    TimeoutCallback cb = [](const Timestamp &timestamp)
    {
        LOG_INFO("timeout : %s", timestamp.to_string().c_str());
    };


    timer.set_timer(1, 1000, cb);

    loop.run();

}
