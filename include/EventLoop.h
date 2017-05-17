#ifndef LIBNET_DISTRIBUTION_EVENTLOOP_H
#define LIBNET_DISTRIBUTION_EVENTLOOP_H
#include <pthread.h>
#include <vector>
namespace net
{

class Selector;
class SelectionKey;

class EventLoop
{
public:
  explicit EventLoop();

  ~EventLoop();

  void run();

private:
  pthread_t pthread_id_;
  volatile bool is_quit_;
  pthread_mutex_t mutex_;
  Selector* selector_;

  std::vector<SelectionKey*> active_keys_;

};




}

#endif //LIBNET_DISTRIBUTION_EVENTLOOP_H
