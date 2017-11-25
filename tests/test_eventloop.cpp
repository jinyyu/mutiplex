#include <net4cxx/EventLoop.h>
#include <thread>
#include <net4cxx/Logger.h>
#include <unistd.h>
#include <net4cxx/Timestamp.h>

using namespace net4cxx;


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