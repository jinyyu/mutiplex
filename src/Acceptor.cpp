#include "evcpp/Acceptor.h"
#include "evcpp/EventLoop.h"
#include "evcpp/Connection.h"
#include "EventSource.h"

namespace ev
{

Acceptor::Acceptor(EventLoop* loop, int port)
    : Acceptor(loop, InetSocketAddress(port))
{
}

Acceptor::Acceptor(EventLoop* loop, const InetSocketAddress& addr)
    : loop_(loop),
      local_addr_(addr)
{
    server_socket_.reuse_port(true);

    Status status = server_socket_.bind(addr);
    if (!status.is_ok()) {
        throw std::runtime_error(status.to_string());
    }

    accept_event_ = new EventSource(server_socket_.fd());

    accept_event_->enable_reading();
    accept_event_->set_reading_callback([this](uint64_t timestamp){
        int fd = server_socket_.accept(peer_addr_);

        if (this->on_new_connection_) {
            this->on_new_connection_(fd, timestamp, local_addr_, this->peer_addr_);
        }
    });

    loop_->register_event(accept_event_);
}

Acceptor::~Acceptor()
{
    delete (accept_event_);
}

}