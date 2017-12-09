#ifndef NET4CXX_DISTRIBUTION_TIMER_H
#define NET4CXX_DISTRIBUTION_TIMER_H
#include <sys/timerfd.h>
#include <string.h>

#include <inttypes.h>
#include <net4cxx/reactor/Channel.h>
#include <boost/noncopyable.hpp>
#include <net4cxx/common/callbacks.h>

namespace net4cxx
{
class EventLoop;

class Timer: boost::noncopyable
{
public:
    explicit Timer(EventLoop* loop);

    ~Timer();

    void set_timer(uint32_t millisecond, const TimeoutCallback& timeout_callback);

    void set_timer(uint32_t millisecond, uint32_t millisecond_interval, const TimeoutCallback& timeout_callback);

private:

    void set_timmer(struct itimerspec spec, const TimeoutCallback& timeout_callback);

    void handle_timeout(const Timestamp& timestamp, SelectionKey* key, const TimeoutCallback& callback);

private:
    int fd_;
    Channel channel_;
    EventLoop* loop_;
};

}

#endif
