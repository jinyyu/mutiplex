#include <libnet/Timer.h>
#include <libnet/EventLoop.h>
#include <libnet/Logger.h>
#include <libnet/Timestamp.h>

using namespace net;

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
