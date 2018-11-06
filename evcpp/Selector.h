#pragma once
#include <boost/noncopyable.hpp>
#include <evcpp/Timestamp.h>
#include <vector>
#include <sys/epoll.h>
#include <pthread.h>

namespace ev
{

class SelectionKey;

class Selector: boost::noncopyable
{
public:
    explicit Selector(pthread_t pthread_id);

    ~Selector();

    void add(SelectionKey* selection_key);

    void modify(SelectionKey* selection_key);

    void remove(SelectionKey* selection_key);

    Timestamp select(int timeout_milliseconds, std::vector<SelectionKey*>& active_key);


private:
    void control(int op, SelectionKey* selection_key);

    pthread_t pthread_id_;
    int epoll_fd_;
    std::vector<struct epoll_event> events_;
    int selecting_events_;

};

}
