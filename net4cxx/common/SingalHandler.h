#ifndef NET4CXX_DISTRIBUTION_SINGALHANDLER_H
#define NET4CXX_DISTRIBUTION_SINGALHANDLER_H
#include <signal.h>
#include <functional>
#include <net4cxx/common/callbacks.h>

namespace net4cxx
{

void AddSignalHandler(int signum, Callback callback);

}



#endif //NET4CXX_DISTRIBUTION_SINGALHANDLER_H
