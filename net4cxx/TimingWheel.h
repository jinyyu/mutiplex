#ifndef NET4CXX_DISTRIBUTION_TIMINGWHEEL_H
#define NET4CXX_DISTRIBUTION_TIMINGWHEEL_H

#include <net4cxx/NonCopyable.h>
#include <net4cxx/Timer.h>

#include <queue>
#include <memory>
#include <unordered_set>

namespace net4cxx
{

class EventLoop;
class Connection;

typedef std::weak_ptr<Connection> WeakConnection;

typedef std::shared_ptr<Connection> SharedConnection;

class ConnectionEntry
{
public:
    ConnectionEntry(WeakConnection conn)
        : conn_(conn)
    {
    }

    ~ConnectionEntry();

private:
    WeakConnection conn_;

};

typedef std::shared_ptr<ConnectionEntry> SharedConnectionEntry;

typedef std::weak_ptr<ConnectionEntry> WeakConnectionEntry;

typedef std::unordered_set<SharedConnectionEntry> Bucket;

class TimingWheel: NonCopyable
{
public:
    explicit TimingWheel(EventLoop *loop, int timeout_second);

    ~TimingWheel();

    void set_default_timeout(SharedConnectionEntry entry);

private:
    void handle_timeout();

private:

    Timer timer_;
    int queue_size_;
    std::queue<Bucket> queue_;

};

}
#endif //LIBNET_DISTRIBUTION_TIMINGWHEEL_H
