#pragma once

#include <memory>

namespace ev
{

class Context
{
public:
    virtual ~Context() = 0;
};

typedef std::unique_ptr<Context> ContextPtr;

}

