#ifndef NET4CXX_DISTRIBUTION_CONTEXT_H
#define NET4CXX_DISTRIBUTION_CONTEXT_H

#include <memory>

namespace net4cxx
{

class Context
{
public:
    virtual ~Context() = 0;
};

typedef std::unique_ptr<Context> ContextPtr;

}

#endif
