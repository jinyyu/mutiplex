#pragma once
#include <signal.h>
#include <functional>
#include <evcpp/callbacks.h>

namespace ev
{

void AddSignalHandler(int signum, Callback callback);

}

