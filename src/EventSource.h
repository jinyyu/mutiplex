#pragma once

#include <stdint.h>
#include <sys/epoll.h>
#include <mutiplex/callbacks.h>
namespace muti
{


typedef std::function<void(uint64_t)> EventCallback;

class EventLoop;
class EventSource
{
public:
    explicit EventSource(int fd, EventLoop* loop)
        : ready_ops_(0),
          interest_ops_(0),
          fd_(fd),
          state_(StateNoEvent),
          loop_(loop)
    {

    }

    ~EventSource();

    int fd()
    {
        return fd_;
    }

    void disable_all();

    void enable_reading()
    {
        enable_ops(EPOLLIN);
    }

    void set_reading_callback(const EventCallback& cb)
    {
        read_callback_ = cb;
    }

    void disable_reading()
    {
        disable_ops(EPOLLIN);
    }

    void enable_writing()
    {
        enable_ops(EPOLLOUT);
    }

    void set_writing_callback(const EventCallback& cb)
    {
        write_callback_ = cb;
    }

    void disable_writing()
    {
        disable_ops(EPOLLOUT);
    }

    void set_close_callback(const EventCallback& cb)
    {
        close_callback_ = cb;
    }

    void set_error_callback(const EventCallback& cb)
    {
        error_callback_ = cb;
    }

    void handle_events(uint64_t timestamp)
    {
        if (ready_ops_ & EPOLLERR && error_callback_) {
            error_callback_(timestamp);
            disable_all();
            return;
        }

        if (ready_ops_ & EPOLLHUP && close_callback_) {
            close_callback_(timestamp);
            disable_all();
            return;
        }

        if (ready_ops_ & EPOLLIN && read_callback_) {
            read_callback_(timestamp);
        }
        if (ready_ops_ & EPOLLOUT && write_callback_) {
            write_callback_(timestamp);
        }
    }

    uint32_t ready_ops() const
    {
        return ready_ops_;
    }

    void ready_ops(uint32_t ops)
    {
        ready_ops_ = ops;
    }

    uint32_t interest_ops() const
    {
        return interest_ops_;
    }

    void interest_ops(uint32_t ops)
    {
        interest_ops_ = ops;
    }

private:

    void enable_ops(uint32_t op);

    void disable_ops(uint32_t op);

    enum State
    {
        StateNoEvent = 0,
        StateRegistered = 1,
    };

    uint8_t state_;
    EventLoop* loop_;
    int fd_;
    uint32_t interest_ops_;
    uint32_t ready_ops_;
    EventCallback read_callback_;
    EventCallback write_callback_;
    EventCallback close_callback_;
    EventCallback error_callback_;
};

}