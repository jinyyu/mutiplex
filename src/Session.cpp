#include "evcpp/Session.h"
#include <unistd.h>
#include "EventSource.h"
#include "evcpp/Timestamp.h"
#include "evcpp/Connection.h"
#include "evcpp/EventLoop.h"
#include "Debug.h"

namespace ev
{

Session::Session(EventLoop* loop, const InetSocketAddress& local)
    : loop_(loop),
      local_(local),
      event_source_(nullptr)
{
    fd_ = ::socket(local.family(), SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (fd_ < 0) {
        LOG_DEBUG("socket error %d", errno);
    }
}

Session::~Session()
{
    if (event_source_) {
        delete event_source_;
    }
}

void Session::connect(const InetSocketAddress& peer)
{
    peer_ = peer;
    event_source_ = new EventSource(fd_);
    event_source_->set_writing_callback([this](uint64_t timestamp){
        handle_connected(timestamp);
    });

    event_source_->set_error_callback([this](uint64_t timestamp){
        handle_error(timestamp);
    });

    loop_->register_event(event_source_);

    if (!do_connect(peer)) {
        //handle connect error
        handle_error(Timestamp::current());
        return;
    }
    else {
        event_source_->enable_writing();
    }
}

void Session::handle_connected(uint64_t timestamp)
{
    event_source_->interest_ops(0);
    loop_->unregister_event(event_source_);


    int err;
    socklen_t len = sizeof(err);
    getsockopt(fd_, SOL_SOCKET, SO_ERROR, &err, &len);

    if (err != 0) {
        //error
        LOG_DEBUG("connect error %d", err);
        handle_error(timestamp);
        return;

    }

    //success
    ConnectionPtr conn(new Connection(fd_, loop_, local_, peer_));

    conn->set_established_callback(connection_established_callback_);
    conn->read_message_callback(read_message_callback_);
    conn->connection_closed_callback(connection_closed_callback_);

    loop_->on_new_connection(conn, timestamp);
}

bool Session::do_connect(const InetSocketAddress& addr)
{
    int len = addr.family() == AF_INET ? sizeof(addr.sockaddr_) : sizeof(addr.sockaddr6_);
    int ret = ::connect(fd_, addr.sockaddr_cast(), len);
    int err = errno;

    if (ret < 0 && err != EINPROGRESS) {
        LOG_DEBUG("connect error %d", errno);
        return false;
    }
    return true;

}

void Session::handle_error(uint64_t timestamp)
{
    if (connect_error_callback_) {
        connect_error_callback_(timestamp);
    }
    ::close(fd_);
}

}