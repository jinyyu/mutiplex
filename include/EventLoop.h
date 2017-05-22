#ifndef LIBNET_DISTRIBUTION_EVENTLOOP_H
#define LIBNET_DISTRIBUTION_EVENTLOOP_H
#include <pthread.h>
#include <vector>
#include <unordered_map>

#include "callbacks.h"
#include "NonCopyable.h"

namespace net
{

class Channel;
class Selector;
class SelectionKey;
class ByteBuffer;

class EventLoop : NonCopyable
{
public:
  explicit EventLoop();

  ~EventLoop();

  //Run the EventLoop object's event processing loop
  void run();

  //Request the EventLoop to invoke the given callback and return immediately
  void post(const Callback& callback);

  void on_new_connection(int fd, const Timestamp& timestamp, const InetSocketAddress& local, const InetSocketAddress& peer);

  void connection_established_callback(const ConnectionEstablishedCallback& cb) { established_callback_ = cb;}

  void read_message_callback(const ReadMessageCallback& cb)  { read_message_callback_ = cb; }

  void connection_closed_callback(const ConnectionClosedCallback& cb) { connection_closed_callback_ = cb; }

  void allocate_receive_buffer(uint32_t capacity);

  void stop();

  bool is_in_loop_thread() const { return pthread_id_ == pthread_self(); }

private:
  friend class Connection;
  Selector* selector() const { return selector_; }

  void remove_connection(int fd) { connections_.erase(fd); }

  void wake_up();

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

  std::unordered_map<int, ConnectionPtr> connections_;

  ConnectionEstablishedCallback established_callback_;
  ReadMessageCallback read_message_callback_;
  ConnectionClosedCallback connection_closed_callback_;

  pthread_mutex_t mutex_running_;

  ByteBuffer* recv_buffer_;

};


}

#endif //LIBNET_DISTRIBUTION_EVENTLOOP_H
