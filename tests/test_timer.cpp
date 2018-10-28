#include <libreactor/Timer.h>
#include <libreactor/EventLoop.h>
#include <libreactor/Timestamp.h>

using namespace reactor;

int main(int argc, char* argv[])
{
    EventLoop loop;
    Timer timer(&loop);

    TimeoutCallback cb = [](const Timestamp& timestamp) {
        fprintf(stderr, "timeout : %s\n", timestamp.to_string().c_str());
    };


    timer.set_timer(1, 1000, cb);

    loop.run();

}
