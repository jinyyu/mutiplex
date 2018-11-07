#pragma once
#include <pthread.h>
#include <vector>
#include <deque>
#include <unordered_map>
#include <sys/epoll.h>
#include <mutiplex/callbacks.h>
#include <boost/noncopyable.hpp>
#include <mutex>

namespace muti
{

class ByteBuffer;
class EventSource;

class EventLoop: boost::noncopyable
{
public:
    explicit EventLoop();

    ~EventLoop();

    //Run the EventLoop object's event processing loop
    void run();

    //Request the EventLoop to invoke the given callback and return immediately
    void post_callback(const Callback& callback);

    void on_new_connection(ConnectionPtr& conn, uint64_t timestamp);

    void allocate_receive_buffer(uint32_t capacity);

    void stop();
private:
    friend class Connection;
    friend class EventSource;
    void remove_connection(int fd)
    {
        connections_.erase(fd);
    }

    void register_event(EventSource* ev);

    void unregister_event(EventSource* ev);

    void modify_event(EventSource* ev);

    void notify_event();

    int pull_events(std::vector<EventSource*>& events);

private:
    pthread_t pthread_id_;
    volatile bool running_;

    friend class Acceptor;
    friend class Session;

    int epoll_fd_;

    int notify_fd_;
    EventSource* notify_event_;

    std::vector<struct epoll_event> epoll_events_;

    std::mutex mutex_;
    std::vector<Callback> pending_callbacks_; //lock by mutex_

    std::deque<Callback> task_queue_;

    std::unordered_map<int, ConnectionPtr> connections_;

    ByteBuffer* recv_buffer_;
};

}

