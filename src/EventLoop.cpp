#include "net4cxx/EventLoop.h"
#include "net4cxx/utils.h"
#include "net4cxx/Selector.h"
#include "net4cxx/Channel.h"
#include "net4cxx/SelectionKey.h"
#include "net4cxx/InetSocketAddress.h"
#include "net4cxx/Connection.h"
#include "net4cxx/ByteBuffer.h"
#include <log4cxx/logger.h>

#include <sys/eventfd.h>
#include <unistd.h>

namespace net4cxx
{
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

EventLoop::EventLoop()
    : pthread_id_(pthread_self()),
      is_quit_(false),
      selector_(new Selector(pthread_id_)),
      active_keys_(128),
      wakeup_fd_(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)),
      recv_buffer_(nullptr)
{
    pthread_mutex_init(&mutex_, NULL);

    active_keys_.reserve(128);

    if (wakeup_fd_ == -1) {
        LOG4CXX_ERROR(logger, "eventfd error " << errno);
    }

    //setup wakeup channel
    wakeup_channel_ = new Channel(selector_, wakeup_fd_);
    wakeup_channel_->enable_reading([this](const Timestamp &, SelectionKey *)
                                    {
                                        uint64_t n;
                                        if (eventfd_read(wakeup_fd_, &n) < 0) {
                                            LOG4CXX_ERROR(logger, "eventfd_read error " << errno);
                                        }
                                    });
}

EventLoop::~EventLoop()
{
    pthread_mutex_destroy(&mutex_);
    delete (wakeup_channel_);
    ::close(wakeup_fd_);

    connections_.clear();

    delete (selector_);

    if (recv_buffer_) {
        delete (recv_buffer_);
    }
    LOG4CXX_INFO(logger, "loop delete " << pthread_id_);

}

void EventLoop::run()
{
    pthread_id_ = pthread_self();
    while (!is_quit_) {
        active_keys_.clear();
        Timestamp time = selector_->select(8000, active_keys_);
        if (active_keys_.empty()) {
            continue;
        }

        for (SelectionKey *key: active_keys_) {
            Channel *channel = key->channel();
            if (key->is_error()) {
                channel->handle_error(time);
            }
            else {
                if (key->is_readable()) {
                    channel->handle_read(time);
                }
                if (key->is_writable()) {
                    channel->handle_wirte(time);
                }
            }
        }

        std::vector<Callback> callbacks;
        {
            MutexLockGuard lock(&mutex_);
            std::swap(callbacks, callbacks_);
        }
        for (int i = 0; i < callbacks.size(); ++i) {
            callbacks[i]();
        }
    }
}

void EventLoop::stop()
{
    is_quit_ = true;
}

void EventLoop::wake_up()
{
    if (is_in_loop_thread())
        return;

    if (eventfd_write(wakeup_fd_, 1) < 0) {
        LOG4CXX_ERROR(logger, "eventfd_write error " << errno);
    }
}

void EventLoop::post(const Callback &callback)
{
    if (is_in_loop_thread()) {
        callback();
    }
    else {
        {
            MutexLockGuard lock(&mutex_);
            callbacks_.push_back(callback);
        }
        wake_up();
    }
}

void EventLoop::on_new_connection(ConnectionPtr& conn, const Timestamp &timestamp)
{
    conn->setup_callbacks();

    connections_[conn->fd()] = conn;

    if (conn->connection_established_callback_) {
        conn->connection_established_callback_(conn, timestamp);
    }
}

void EventLoop::allocate_receive_buffer(uint32_t capacity)
{
    recv_buffer_ = new ByteBuffer(capacity);
}

}