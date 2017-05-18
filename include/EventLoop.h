#ifndef LIBNET_DISTRIBUTION_EVENTLOOP_H
#define LIBNET_DISTRIBUTION_EVENTLOOP_H
#include <pthread.h>
#include <vector>
namespace net
{

class Channel;
class Selector;
class SelectionKey;

class EventLoop
{
public:
  explicit EventLoop();

  ~EventLoop();

  void run();

  void wake_up();

private:
  void setup_wakeup_channel();

  bool is_in_loop_thread() const { return pthread_id_ == pthread_self(); }

private:
  pthread_t pthread_id_;
  volatile bool is_quit_;
  Selector* selector_;
  std::vector<SelectionKey*> active_keys_;


  int wakeup_fd_;
  Channel* wakeup_channel_;

  pthread_mutex_t mutex_;

};




}

#endif //LIBNET_DISTRIBUTION_EVENTLOOP_H
