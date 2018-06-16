#include "net4cxx/common/Status.h"
#include "net4cxx/common/InetSocketAddress.h"
#include <log4cxx/logger.h>


namespace reactor
{

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

InetSocketAddress::InetSocketAddress()
{
    from_address(InetAddress::any(AF_INET), 0);
}

InetSocketAddress::InetSocketAddress(const InetAddress& addr, int port)
{
    from_address(addr, port);
}

void InetSocketAddress::from_address(const InetAddress& addr, int port)
{
    if (addr.v4()) {
        sockaddr_.sin_port = htons(port);
        sockaddr_.sin_family = AF_INET;
        sockaddr_.sin_addr = addr.addr_;

    }
    else {
        sockaddr6_.sin6_family = AF_INET6;
        sockaddr6_.sin6_port = htons(port);
        sockaddr6_.sin6_addr = addr.addr6_;
    }
}

InetSocketAddress::InetSocketAddress(int port)
    : InetSocketAddress(InetAddress::any(AF_INET), port)
{
}

InetSocketAddress::InetSocketAddress(const char* hostname, int port)
{
    Status s;
    InetAddress addr = InetAddress::get_by_host(hostname, s);
    if (!s.is_ok()) {
        LOG4CXX_ERROR(logger, "invalid hostname " << hostname);
    }

    from_address(addr, port);
}

int InetSocketAddress::port() const
{
    sockaddr* p = (sockaddr*) &(sockaddr_);
    if (p->sa_family == AF_INET) {
        return ntohs(sockaddr_.sin_port);
    }
    else {
        return ntohs(sockaddr6_.sin6_port);
    }
}

InetAddress InetSocketAddress::get_address() const
{

    int family = sockaddr_cast()->sa_family;
    InetAddress addr(family);
    sockaddr* p = (sockaddr*) &(sockaddr_);
    if (p->sa_family == AF_INET) {
        addr.addr_ = sockaddr_.sin_addr;
    }
    else {
        addr.addr6_ = sockaddr6_.sin6_addr;
    }

    return addr;
}

std::string InetSocketAddress::to_string() const
{
    return get_address().to_string() + ":" + std::to_string(port());
}

}