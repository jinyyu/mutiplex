#include "libreactor/Session.h"
#include <unistd.h>

#include "libreactor/Timestamp.h"
#include "libreactor/Connection.h"
#include "libreactor/Selector.h"
#include "libreactor/SelectionKey.h"
#include "libreactor/EventLoop.h"
#include "libreactor/Channel.h"

#include <log4cxx/logger.h>

namespace reactor
{

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

Session::Session(EventLoop* loop, const InetSocketAddress& local)
    : loop_(loop),
      local_(local),
      channel_(nullptr)
{
    fd_ = ::socket(local.family(), SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (fd_ < 0) {
        LOG4CXX_ERROR(logger, "socket error " << errno);
    }
}

Session::~Session()
{
    if (channel_) {
        delete channel_;
    }
}

void Session::connect(const InetSocketAddress& peer)
{
    peer_ = peer;
    channel_ = new Channel(loop_->selector_, fd_);
    SelectionCallback write_cb = [this](const Timestamp& timestamp, SelectionKey* key)
    {
        this->handle_connected(timestamp, key);
    };
    channel_->set_writing_selection_callback(write_cb);


    if (!do_connect(peer)) {
        //handle connect error
        handle_error(Timestamp::currentTime());
        return;
    }
    else {
        channel_->enable_writing();
    }
}

void Session::handle_connected(const Timestamp& timestamp, SelectionKey* key)
{
    channel_->disable_all();

    if (key->is_error()) {
        LOG4CXX_INFO(logger, "error");
        handle_error(timestamp);
        return;
    }

    int err;
    socklen_t len = sizeof(err);
    getsockopt(fd_, SOL_SOCKET, SO_ERROR, &err, &len);

    if (err != 0) {
        //error
        LOG4CXX_ERROR(logger, "connect error " << err);
        handle_error(timestamp);
        return;

    }

    //success
    ConnectionPtr conn(new Connection(fd_, loop_, local_, peer_));

    conn->connection_established_callback(connection_established_callback_);
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
        LOG4CXX_ERROR(logger, "connect error " << errno);
        return false;
    }
    return true;

}

void Session::handle_error(const Timestamp& timestamp)
{
    LOG4CXX_ERROR(logger, "error happened " << timestamp.to_string());
    if (connect_error_callback_) {
        connect_error_callback_(timestamp);
    }
    ::close(fd_);
}

}