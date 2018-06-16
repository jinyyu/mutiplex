#include "libreactor/ServerSocket.h"
#include "libreactor/InetSocketAddress.h"
#include "libreactor/InetAddress.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <log4cxx/logger.h>


namespace reactor
{

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("net4cxx"));

ServerSocket::ServerSocket()
    : fd_(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP))
{
    if (fd_ < 0) {
        LOG4CXX_ERROR(logger, "create socket error " << errno);
    }
}

ServerSocket::~ServerSocket()
{
    ::close(fd_);
}

Status ServerSocket::bind(const InetSocketAddress& addr)
{
    int ret = ::bind(fd_, addr.sockaddr_cast(), sizeof(sockaddr_in6));
    if (ret < 0) {
        LOG4CXX_ERROR(logger, "bind InetSocketAddress error" << addr.get_address().to_string() << " " << addr.port());
    }
    listen();

    return ret < 0 ? Status::invalid_argument("invalid address") : Status::ok();
}

void ServerSocket::listen()
{
    if (::listen(fd_, SOMAXCONN) < 0) {
        LOG4CXX_ERROR(logger, "listen error " << errno);
    }
}

int ServerSocket::accept(InetSocketAddress& addr)
{
    uint32_t len = sizeof(addr.sockaddr6_);
    int fd = ::accept4(fd_, addr.sockaddr_cast(), &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (fd < 0) {
        LOG4CXX_ERROR(logger, "accept4 error " << errno);
    }

    return fd;

}

void ServerSocket::reuse_address(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void ServerSocket::reuse_port(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
}

}