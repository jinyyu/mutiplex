#include "evcpp/Acceptor.h"
#include "evcpp/EventLoop.h"
#include "evcpp/Connection.h"
#include "EventSource.h"

namespace ev
{

Acceptor::Acceptor(EventLoop* loop, const std::string& addr_str)
    : Acceptor(loop, InetAddress(addr_str))
{
}

Acceptor::Acceptor(EventLoop* loop, const InetAddress& addr)
    : loop_(loop),
      local_addr_(addr)
{
    server_socket_.reuse_port(true);

    server_socket_.bind(addr);

    accept_event_ = new EventSource(server_socket_.fd(), loop_);

    accept_event_->enable_reading();
    accept_event_->set_reading_callback([this](uint64_t timestamp){
        int fd = server_socket_.accept(peer_addr_);

        if (this->on_new_connection_) {
            this->on_new_connection_(fd, timestamp, local_addr_, this->peer_addr_);
        }
    });
}

Acceptor::~Acceptor()
{
    delete (accept_event_);
}

}