#ifndef LIBNET_DISTRIBUTION_EVENTLOOP_H
#define LIBNET_DISTRIBUTION_EVENTLOOP_H
#include <pthread.h>
#include <vector>

#include "callbacks.h"
#include "NonCopyable.h"

namespace net
{

class Channel;
class Selector;
class SelectionKey;

class EventLoop : NonCopyable
{
public:
  explicit EventLoop();

  ~EventLoop();

  //Run the EventLoop object's event processing loop
  void run();

  //Request the EventLoop to invoke the given callback and return immediately
  void post(const Callback& callback);

  void on_new_connection(int fd);

private:
  void setup_wakeup_channel();

  bool is_in_loop_thread() const { return pthread_id_ == pthread_self(); }

  void wake_up();

  void stop()
  {
    is_quit_ = true;
    wake_up();
  }

private:
  pthread_t pthread_id_;
  volatile bool is_quit_;

  friend class Acceptor;
  Selector* selector_;
  std::vector<SelectionKey*> active_keys_;

  int wakeup_fd_;
  Channel* wakeup_channel_;

  pthread_mutex_t mutex_;
  std::vector<Callback> callbacks_; //lock by mutex_

};


}

#endif //LIBNET_DISTRIBUTION_EVENTLOOP_H
