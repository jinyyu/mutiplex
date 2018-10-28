#include <libreactor/EventLoop.h>
#include <thread>
#include <unistd.h>
#include <libreactor/Timestamp.h>

using namespace reactor;


auto fun = []() {
    printf("%s\n", Timestamp::currentTime().to_string().c_str());
};

int main(int argc, char* argv[])
{

    EventLoop loop;

    EventLoop* p = &loop;

    std::thread thread([p]() {
        sleep(3);

        p->post(fun);
    });


    p->post(fun);
    loop.run();
}