#pragma once

#include <functional>
#include <memory>

namespace ev
{

class Timestamp;
class SelectionKey;
class InetSocketAddress;
class Connection;
class ByteBuffer;

typedef std::shared_ptr<Connection> ConnectionPtr;

typedef std::function<void(uint64_t timestamp, SelectionKey*)> SelectionCallback;

typedef std::function<void()> Callback;

typedef std::function<void(int fd, uint64_t timestamp, const InetSocketAddress&, const InetSocketAddress&)>
    NewConnectionCallback;

typedef std::function<void(ConnectionPtr conn, uint64_t timestamp)> ConnectionEstablishedCallback;

typedef std::function<void(ConnectionPtr conn, ByteBuffer*, uint64_t timestamp)> ReadMessageCallback;

typedef std::function<void(ConnectionPtr conn, uint64_t timestamp)> ConnectionClosedCallback;

typedef std::function<void(ConnectionPtr conn, uint64_t timestamp)> ErrorCallback;

typedef std::function<void(uint64_t timestamp)> ConnectErrorCallback;

typedef std::function<void(uint64_t timestamp)> TimeoutCallback;

}

