#include "libnet/TimingWheel.h"
#include "libnet/EventLoop.h"
#include "libnet/Connection.h"
#include "libnet/Logger.h"


namespace net
{

ConnectionEntry::~ConnectionEntry()
{
    SharedConnection conn = conn_.lock();
    if (conn) {
        conn->close();
    }
}

TimingWheel::TimingWheel(EventLoop *loop, int timeout_second)
    : timer_(loop),
      queue_size_(timeout_second)
{
    for (int i = 0; i < queue_size_; ++i) {
        queue_.push(Bucket());
    }

    TimeoutCallback cb = [this](const Timestamp &timestamp)
    {
        this->handle_timeout();
    };

    timer_.set_timer(1, 1000, cb);
}

TimingWheel::~TimingWheel()
{

}

void TimingWheel::handle_timeout()
{
    queue_.pop();
    queue_.push(Bucket());
}

void TimingWheel::set_default_timeout(ConnectionPtr conn)
{
    SharedConnectionEntry entry = conn->get_context().lock();
    if (entry) {
        queue_.back().insert(entry);
    }
}

}
