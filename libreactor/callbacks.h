#pragma once

#include <functional>
#include <memory>

namespace reactor
{

class Timestamp;
class SelectionKey;
class InetSocketAddress;
class Connection;
class ByteBuffer;

typedef std::shared_ptr<Connection> ConnectionPtr;

typedef std::function<void(const Timestamp&, SelectionKey*)> SelectionCallback;

typedef std::function<void()> Callback;

typedef std::function<void(int, const Timestamp&, const InetSocketAddress&, const InetSocketAddress&)>
    NewConnectionCallback;

typedef std::function<void(ConnectionPtr, const Timestamp&)> ConnectionEstablishedCallback;

typedef std::function<void(ConnectionPtr, ByteBuffer*, const Timestamp&)> ReadMessageCallback;

typedef std::function<void(ConnectionPtr, const Timestamp&)> ConnectionClosedCallback;

typedef std::function<void(ConnectionPtr, const Timestamp&)> ErrorCallback;

typedef std::function<void(const Timestamp&)> ConnectErrorCallback;

typedef std::function<void(const Timestamp&)> TimeoutCallback;

}

