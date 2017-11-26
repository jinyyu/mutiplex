#include "net4cxx/common/SingalHandler.h"
#include <unordered_map>
#include <log4cxx/logger.h>

namespace net4cxx
{

static std::unordered_map<int, Callback> g_callback_map;
static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

static void handle_signal(int signum)
{
    if (g_callback_map.find(signum) == g_callback_map.end()) {
        LOG4CXX_ERROR(logger, "signum not found " << signum);
        return;
    }
    else {
        LOG4CXX_INFO(logger, "signal " << signum);
        g_callback_map[signum]();
        g_callback_map.erase(signum);
    }
}

void AddSignalHandler(int signum, Callback callback)
{
    if (g_callback_map.find(signum) != g_callback_map.end()) {
        LOG4CXX_WARN(logger, "signum already set " << signum);
    }
    g_callback_map[signum] = callback;
    signal(signum, handle_signal);
}

}
