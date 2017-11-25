#include <net4cxx/EventLoop.h>
#include <thread>
#include <unistd.h>
#include <net4cxx/Timestamp.h>

using namespace net4cxx;


auto fun = []()
{
    printf("%s\n", Timestamp::currentTime().to_string().c_str());
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