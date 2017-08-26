#include "libnet/Timer.h"

#include <unistd.h>

#include "libnet/EventLoop.h"
#include "libnet/Logger.h"
#include "libnet/Timestamp.h"

namespace net
{

Timer::Timer(EventLoop *loop)
    : fd_(timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC)),
      channel_(loop->selector(), fd_),
      loop_(loop)
{
    if (fd_ < 0) {
        LOG_ERROR("timerfd_create error %d", errno);
    }
}

Timer::~Timer()
{
    ::close(fd_);
}

void Timer::set_timer(uint32_t millisecond, const TimeoutCallback &timeout_callback)
{
    if (millisecond == 0) {
        Callback cb = [timeout_callback]()
        {
            timeout_callback(Timestamp::currentTime());
        };
        loop_->post(cb);
        return;
    }
    struct itimerspec spec;
    memset(&spec, 0, sizeof(spec));

    spec.it_value.tv_sec = millisecond / 1000;
    spec.it_value.tv_nsec = millisecond % (1000 * 1000);

    set_timmer(spec, timeout_callback);
}


void Timer::set_timer(uint32_t millisecond, uint32_t millisecond_interval, const TimeoutCallback &timeout_callback)
{
    struct itimerspec spec;
    memset(&spec, 0, sizeof(spec));
    if (millisecond == 0 || millisecond_interval == 0) {
        LOG_ERROR("set_timer error %lu, %lu", millisecond, millisecond_interval);
    }

    spec.it_value.tv_sec = millisecond / 1000;
    spec.it_value.tv_nsec = millisecond % (1000 * 1000);

    spec.it_interval.tv_sec = millisecond_interval / 1000;
    spec.it_interval.tv_nsec = millisecond_interval % (1000 * 1000);

    set_timmer(spec, timeout_callback);
}

void Timer::set_timmer(struct itimerspec spec, const TimeoutCallback &timeout_callback)
{
    if (timerfd_settime(fd_, 0, &spec, NULL) < 0) {
        LOG_ERROR("timerfd_settime error %d", errno);
    }

    SelectionCallback cb = [this, timeout_callback](const Timestamp &timestamp, SelectionKey *key)
    {
        this->handle_timeout(timestamp, key, timeout_callback);
    };
    channel_.enable_reading(cb);
}

void Timer::handle_timeout(const Timestamp &timestamp, SelectionKey *key, const TimeoutCallback &callback)
{
    uint64_t exp;
    if (read(fd_, &exp, sizeof(uint64_t)) != sizeof(uint64_t)) {
        LOG_ERROR("read error %d", errno);
    }
    callback(timestamp);

}

}
