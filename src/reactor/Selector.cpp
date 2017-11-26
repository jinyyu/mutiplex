#include "net4cxx/reactor/Selector.h"
#include <unistd.h>
#include <memory.h>
#include "net4cxx/reactor/Channel.h"
#include "net4cxx/reactor/SelectionKey.h"

#include <log4cxx/logger.h>


namespace net4cxx
{
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

Selector::Selector(pthread_t pthread_id)
    : events_(64),
      selecting_events_(0),
      pthread_id_(pthread_id)
{
    epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd_ == -1) {
        LOG4CXX_ERROR(logger, "epoll_create1 error " << errno);
    }
}

Selector::~Selector()
{
    ::close(epoll_fd_);
}

void Selector::add(SelectionKey* selection_key)
{
    control(EPOLL_CTL_ADD, selection_key);
    ++selecting_events_;
}

void Selector::modify(SelectionKey* selection_key)
{
    control(EPOLL_CTL_MOD, selection_key);
}

void Selector::remove(SelectionKey* selection_key)
{
    control(EPOLL_CTL_DEL, selection_key);
    --selecting_events_;
}

void Selector::control(int op, SelectionKey* selection_key)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.ptr = selection_key;
    event.events = selection_key->interest_ops();

    if (epoll_ctl(epoll_fd_, op, selection_key->fd(), &event) != 0) {
        LOG4CXX_ERROR(logger, "epoll_ctl error, op  " << op << " error " << errno);
    }
}

Timestamp Selector::select(int timeout_milliseconds, std::vector<SelectionKey*>& active_key)
{
    int n_events = epoll_wait(epoll_fd_, events_.data(), events_.size(), timeout_milliseconds);
    Timestamp cur = Timestamp::currentTime();
    if (n_events == -1) {
        LOG4CXX_ERROR(logger, "epoll_wait error " << errno);
    }
    else if (n_events == 0) {
        return cur;
    }
    else {
        for (int i = 0; i < n_events; ++i) {
            SelectionKey* key = (SelectionKey*) events_[i].data.ptr;
            key->ready_ops(events_[i].events);
            active_key.push_back(key);
        }
        if (n_events == events_.size()) {
            events_.resize(n_events * 2);
        }
    }
    return cur;
}

}