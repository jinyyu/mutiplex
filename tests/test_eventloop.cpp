#include <libnet/EventLoop.h>
#include <thread>
#include <libnet/Logger.h>
#include <unistd.h>
using namespace net;
#include <libnet/Timestamp.h>


auto fun = []()
{
    LOG_INFO("%s", Timestamp::currentTime().to_string().c_str());
};

int main(int argc, char *argv[])
{

    EventLoop loop;

    EventLoop *p = &loop;

    std::thread thread([p]()
                       {
                           sleep(3);

                           p->post(fun);
                       });


    p->post(fun);
    loop.run();
}