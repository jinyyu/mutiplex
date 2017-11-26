#include "net4cxx/reactor/Acceptor.h"
#include "net4cxx/reactor/ServerSocket.h"
#include "net4cxx/common/InetSocketAddress.h"
#include "net4cxx/reactor/Channel.h"
#include "net4cxx/reactor/EventLoop.h"
#include "net4cxx/common/Timestamp.h"
#include "net4cxx/reactor/Connection.h"
#include <log4cxx/logger.h>

namespace net4cxx
{

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

Acceptor::Acceptor(EventLoop* loop, int port)
    : Acceptor(loop, InetSocketAddress(port))
{
}

Acceptor::Acceptor(EventLoop* loop, const InetSocketAddress& addr)
    : loop_(loop),
      local_addr_(new InetSocketAddress(addr))
{
    peer_addr_ = new InetSocketAddress();
    server_socket_ = new ServerSocket();
    server_socket_->reuse_port(true);

    Status status = server_socket_->bind(addr);
    if (!status.is_ok()) {
        LOG4CXX_ERROR(logger, "bind error " << status.to_string());
        exit(EXIT_FAILURE);
    }

    accept_channel_ = new Channel(loop->selector_, server_socket_->fd());

    SelectionCallback cb = [this](const Timestamp& timestamp, SelectionKey* key)
    {
        int fd = server_socket_->accept(*this->peer_addr_);

        if (this->callback_) {
            this->callback_(fd, timestamp, *local_addr_, *this->peer_addr_);
        }
    };

    accept_channel_->enable_reading(cb);

}

Acceptor::~Acceptor()
{
    delete (server_socket_);
    delete (accept_channel_);
    delete (peer_addr_);
    delete (local_addr_);
}

}