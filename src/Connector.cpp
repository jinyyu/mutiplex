#include <EventLoop.h>
#include "Connector.h"
#include "ClientSocket.h"
#include "Timestamp.h"
#include "Connection.h"
#include "Selector.h"
#include "Channel.h"
#include "Logger.h"
#include "SelectionKey.h"

namespace net
{

Connector::Connector(EventLoop* loop, const InetSocketAddress& local)
    : loop_(loop),
      local_(local),
      channel_(nullptr)
{
  client_socket_ = new ClientSocket(local.family());
}


Connector::~Connector()
{
  if (channel_) {
    delete channel_;
  }
  if (client_socket_) {
    delete client_socket_;
  }

}

void Connector::connect(const InetSocketAddress& peer)
{
  peer_ = peer;
  channel_ = new Channel(loop_->selector_, client_socket_->fd());
  SelectionCallback write_cb = [this](const Timestamp & timestamp, SelectionKey* key) {
    this->handle_connected(timestamp, key);
  };
  channel_->set_writing_selection_callback(write_cb);


  if (!client_socket_->connect(peer_)) {
    //handle connect error
    if (connect_error_callback_) {
      connect_error_callback_(Timestamp::currentTime());
    }
    return;
  } else {
    channel_->enable_writing();
  }
}

void Connector::handle_connected(const Timestamp& timestamp, SelectionKey* key)
{
  channel_->disable_writing();

  if (key->is_error()) {
    LOG_INFO("error");
    if (connect_error_callback_) {
      connect_error_callback_(timestamp);
    }
    return;
  }

  int err;
  socklen_t len = sizeof(err);
  getsockopt(client_socket_->fd(), SOL_SOCKET, SO_ERROR, &err, &len);

  if (err != 0) {
    //error
    LOG_INFO("connect error %d", err);
    if (connect_error_callback_) {
      connect_error_callback_(timestamp);
    }
    return;

  }

  LOG_INFO("connect success")
  //success
  ConnectionPtr conn(new Connection(client_socket_->fd(), loop_, local_, peer_));

  conn->connection_established_callback(connection_established_callback_);
  conn->read_message_callback(read_message_callback_);
  conn->connection_closed_callback(connection_closed_callback_);

  loop_->on_new_connection(conn, timestamp);
}



}