#ifndef LIBNET_DISTRIBUTION_TCPCLIENT_H
#define LIBNET_DISTRIBUTION_TCPCLIENT_H

#include "callbacks.h"
#include <InetSocketAddress.h>

namespace net
{

class EventLoop;
class InetSocketAddress;
class Channel;
class ClientSocket;

class TcpClient
{
public:
  explicit TcpClient(EventLoop* loop, const InetSocketAddress& addr);

  ~TcpClient();

  void connect_error_callback(const ConnectErrorCallback& cb) { connect_error_callback_ = cb; }

  void connect();

private:
  void handle_connected(const Timestamp& timestamp);

  void handle_read(ConnectionPtr conn, ByteBuffer* buffer, const Timestamp & timestamp);

private:
  EventLoop* loop_;
  InetSocketAddress peer_;
  Channel* channel_;
  ClientSocket* client_socket_;

  ConnectErrorCallback connect_error_callback_;
  ConnectionEstablishedCallback connection_established_callback_;
  ReadMessageCallback read_message_callback_;
  ConnectionClosedCallback connection_closed_callback_;
};


}


#endif //LIBNET_DISTRIBUTION_TCPCLIENT_H
