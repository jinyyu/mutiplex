#pragma once

#include <functional>
#include <memory>

namespace ev
{
class ByteBuffer;
class InetAddress;
class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;

typedef std::function<void()> Callback;

typedef std::function<void(int fd, uint64_t timestamp, const InetAddress&, const InetAddress&)> NewConnectionCallback;

typedef std::function<void(ConnectionPtr conn, uint64_t timestamp)> EstablishedCallback;

typedef std::function<void(ConnectionPtr conn, ByteBuffer*, uint64_t timestamp)> ReadCallback;

typedef std::function<void(ConnectionPtr conn, uint64_t timestamp)> ClosedCallback;

typedef std::function<void(ConnectionPtr conn, uint64_t timestamp)> ErrorCallback;

typedef std::function<void(uint64_t timestamp)> ConnectErrorCallback;

typedef std::function<void(uint64_t timestamp)> TimeoutCallback;

}

