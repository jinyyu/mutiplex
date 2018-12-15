#include "mutiplex/EventLoop.h"
#include "mutiplex/InetAddress.h"
#include "mutiplex/Connection.h"
#include "mutiplex/ByteBuffer.h"
#include "EventSource.h"
#include "mutiplex/Timestamp.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include "Debug.h"

namespace muti
{

EventLoop::EventLoop()
    : pthread_id_(pthread_self()),
      running_(true),
      epoll_fd_(epoll_create1(EPOLL_CLOEXEC)),
      notify_fd_(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)),
      epoll_events_(64),
      recv_buffer_(nullptr)
{
    if (notify_fd_ == -1) {
        LOG_DEBUG("eventfd error %d", errno);
    }

    notify_event_ = new EventSource(notify_fd_, this);
    notify_event_->enable_reading();
    notify_event_->set_reading_callback([this](uint64_t timestamp) {
        uint64_t n;
        if (eventfd_read(notify_fd_, &n) < 0) {
            LOG_DEBUG("eventfd_read error %d", errno);
        }
    });
}

EventLoop::~EventLoop()
{

    if (recv_buffer_) {
        delete (recv_buffer_);
    }
    connections_.clear();

    delete (notify_event_);
    ::close(notify_fd_);

    LOG_DEBUG("loop delete %lu", pthread_id_);

    ::close(epoll_fd_);
}

void EventLoop::run()
{
    pthread_id_ = pthread_self();
    std::vector<EventSource*> active_events;

    while (running_) {
        active_events.clear();
        int ret = pull_events(active_events);
        if (ret == -1) {
            break;
        }
        uint64_t now = Timestamp::current();

        for (size_t i = 0; i < active_events.size(); ++i) {
            active_events[i]->handle_events(now);
        }

        std::vector<Callback> callbacks;
        {
            std::lock_guard<std::mutex> lock_guard(mutex_);
            std::swap(callbacks, pending_callbacks_);
        }

        for (size_t i = 0; i < callbacks.size(); ++i) {
            callbacks[i]();
        }

        while (!task_queue_.empty()) {
            task_queue_.front()();
            task_queue_.pop_front();
        }
    }
}

void EventLoop::stop()
{
    running_ = false;
    notify_event();
}

void EventLoop::notify_event()
{
    if (eventfd_write(notify_fd_, 1) < 0) {
        LOG_DEBUG("eventfd_write error %d", errno);
    }
}

void EventLoop::register_event(EventSource* ev)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.ptr = ev;
    event.events = ev->interest_ops();

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, ev->fd(), &event) != 0) {
        LOG_DEBUG("epoll_ctl error %s, %d", strerror(errno), ev->fd());
    }
}

void EventLoop::unregister_event(EventSource* ev)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.ptr = ev;
    event.events = ev->interest_ops();

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, ev->fd(), &event) != 0) {
        LOG_DEBUG("epoll_ctl error %d %s", ev->fd(), strerror(errno));
    }
}

void EventLoop::modify_event(EventSource* ev)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.ptr = ev;
    event.events = ev->interest_ops();

    if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, ev->fd(), &event) != 0) {
        LOG_DEBUG("epoll_ctl error %s", strerror(errno));
    }
}

int EventLoop::pull_events(std::vector<EventSource*>& events)
{
    int n_events = ::epoll_wait(epoll_fd_, epoll_events_.data(), epoll_events_.size(), 1000);
    if (n_events == -1) {
        LOG_DEBUG("epoll_wait error %s", strerror(errno));
        return n_events;
    }
    for (int i = 0; i < n_events; ++i) {
        EventSource* ev = (EventSource*) epoll_events_[i].data.ptr;
        ev->ready_ops(epoll_events_[i].events);
        events.push_back(ev);
    }
    if (n_events == (int)epoll_events_.size()) {
        epoll_events_.resize(n_events * 2);
    }
    return n_events;
}

void EventLoop::post_callback(const Callback& callback)
{
    if (pthread_id_ == pthread_self()) {
        task_queue_.push_back(callback);
    }
    else {
        std::lock_guard<std::mutex> guard(mutex_);
        pending_callbacks_.push_back(callback);
        notify_event();
    }
}

void EventLoop::on_new_connection(ConnectionPtr& conn, uint64_t timestamp)
{
    conn->register_event();

    connections_[conn->fd()] = conn;

    if (conn->established_callback_) {
        conn->established_callback_(conn, timestamp);
    }
}

void EventLoop::allocate_receive_buffer(uint32_t capacity)
{
    recv_buffer_ = new ByteBuffer(capacity);
}

}