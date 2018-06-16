#pragma once
#include <signal.h>
#include <functional>
#include <libreactor/callbacks.h>

namespace reactor
{

void AddSignalHandler(int signum, Callback callback);

}

