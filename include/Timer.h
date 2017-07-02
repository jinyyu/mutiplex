#ifndef LIBNET_DISTRIBUTION_TIMER_H
#define LIBNET_DISTRIBUTION_TIMER_H
#include <sys/timerfd.h>
#include <string.h>

#include <inttypes.h>
#include <Channel.h>
#include <NonCopyable.h>
#include <callbacks.h>

namespace net
{
class EventLoop;

class Timer : NonCopyable
{
public:
  Timer(EventLoop* loop);

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

#endif //LIBNET_DISTRIBUTION_TIMER_H
