#include <EventLoop.h>
#include "TcpClient.h"
#include "ClientSocket.h"
#include "InetSocketAddress.h"
#include "Timestamp.h"
#include "Connection.h"
#include "Selector.h"
#include "Channel.h"
#include "Logger.h"

namespace net
{

TcpClient::TcpClient(EventLoop* loop, const InetSocketAddress& addr)
    : loop_(loop),
      peer_(addr),
      channel_(nullptr)
{
  client_socket_ = new ClientSocket(addr.family());
}


TcpClient::~TcpClient()
{
  if (channel_) {
    delete channel_;
  }
  if (client_socket_) {
    delete client_socket_;
  }

}

void TcpClient::connect()
{
  channel_ = new Channel(loop_->selector_, client_socket_->fd());
  SelectionCallback write_cb = [this](const Timestamp & timestamp, SelectionKey *) {
    this->handle_connected(timestamp);
  };
  channel_->set_writing_selection_callback(write_cb);


  if (!client_socket_->connect(peer_)) {
    //handle connect error
    if (connect_error_callback_) {
      connect_error_callback_(Timestamp::currentTime());
    }
  } else {
    channel_->enable_writing();
  }
}

void TcpClient::handle_connected(const Timestamp& timestamp)
{
  LOG_INFO("connect success");
  channel_->disable_writing();
  ConnectionPtr conn(new Connection(client_socket_->fd(), loop_, /*fix me*/peer_, peer_));

  ReadMessageCallback read_cb = [this](ConnectionPtr conn, ByteBuffer* buffer, const Timestamp & timestamp) {
    this->handle_read(conn, buffer, timestamp);
  };
  conn->read_message_callback(read_cb);


}


void TcpClient::handle_read(ConnectionPtr conn, ByteBuffer* buffer, const Timestamp & timestamp)
{

}
}