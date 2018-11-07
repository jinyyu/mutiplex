#pragma once

#include <mutiplex/callbacks.h>
#include <mutiplex/InetAddress.h>

namespace muti
{

class EventLoop;
class EventSource;

class Connector
{
public:
    explicit Connector(EventLoop* loop, const InetAddress& peer);

    ~Connector();

    void connect_error_callback(const ConnectErrorCallback& cb)
    {
        connect_error_callback_ = cb;
    }

    void established_callback(const EstablishedCallback& cb)
    {
        established_callback_ = cb;
    }

    void set_read_callback(const ReadCallback& cb)
    {
        read_callback_ = cb;
    }

    void closed_callback(const ClosedCallback& cb)
    {
        closed_callback_ = cb;
    }

    void start_connect();

private:
    void handle_connected(uint64_t timestamp);

    bool do_connect();

    void handle_error(uint64_t timestamp);

private:
    int fd_;
    EventLoop* loop_;
    InetAddress local_;
    InetAddress peer_;
    EventSource* event_source_;

    ConnectErrorCallback connect_error_callback_;
    EstablishedCallback established_callback_;
    ReadCallback read_callback_;
    ClosedCallback closed_callback_;
};

}

