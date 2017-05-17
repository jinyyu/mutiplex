#ifndef LIBNET_DISTRIBUTION_CALLBACKS_H
#define LIBNET_DISTRIBUTION_CALLBACKS_H

#include <functional>

namespace net
{

class Timestamp;
class SelectionKey;

typedef std::function<void (const Timestamp&, SelectionKey*)> SelectionCallback;

}


#endif //LIBNET_DISTRIBUTION_CALLBACKS_H
