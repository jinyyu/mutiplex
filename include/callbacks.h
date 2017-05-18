#ifndef LIBNET_DISTRIBUTION_CALLBACKS_H
#define LIBNET_DISTRIBUTION_CALLBACKS_H

#include <functional>

namespace net
{

class Timestamp;
class SelectionKey;
class InetSocketAddress;

typedef std::function<void (const Timestamp&, SelectionKey*)> SelectionCallback;

typedef std::function<void ()> Callback;

typedef std::function<void (int, const Timestamp&, const InetSocketAddress&)> NewConnectionCallback;

}


#endif //LIBNET_DISTRIBUTION_CALLBACKS_H
