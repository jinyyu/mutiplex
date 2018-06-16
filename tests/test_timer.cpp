#include <libreactor/Timer.h>
#include <libreactor/EventLoop.h>
#include <libreactor/Timestamp.h>
#include <log4cxx/logger.h>

using namespace reactor;

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

int main(int argc, char *argv[])
{
    EventLoop loop;
    Timer timer(&loop);

    TimeoutCallback cb = [](const Timestamp &timestamp)
    {
        LOG4CXX_INFO(logger, "timeout : " <<  timestamp.to_string());
    };


    timer.set_timer(1, 1000, cb);

    loop.run();

}
