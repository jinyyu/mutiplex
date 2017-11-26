#ifndef NET4CXX_DISTRIBUTION_NONCOPYABLE_H
#define NET4CXX_DISTRIBUTION_NONCOPYABLE_H


namespace net4cxx
{

class NonCopyable
{
public:

    NonCopyable(const NonCopyable& instance) = delete;

    NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

}

#endif