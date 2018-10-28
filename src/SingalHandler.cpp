#include "libreactor/SingalHandler.h"
#include <unordered_map>
#include "Debug.h"

namespace reactor
{

static std::unordered_map<int, Callback> g_callback_map;

static void handle_signal(int signum)
{
    if (g_callback_map.find(signum) == g_callback_map.end()) {
        LOG_DEBUG("signum not found %d", signum);
        return;
    }
    else {
        LOG_DEBUG("signal %d", signum);
        g_callback_map[signum]();
        g_callback_map.erase(signum);
    }
}

void AddSignalHandler(int signum, Callback callback)
{
    if (g_callback_map.find(signum) != g_callback_map.end()) {
        LOG_DEBUG("signum already set %d", signum);
    }
    g_callback_map[signum] = callback;
    signal(signum, handle_signal);
}

}
