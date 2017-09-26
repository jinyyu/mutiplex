#include "libnet/ServerSocket.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "libnet/Logger.h"
#include "libnet/InetSocketAddress.h"
#include "libnet/InetAddress.h"

namespace net
{

ServerSocket::ServerSocket()
    : fd_(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP))
{
    if (fd_ < 0) {
        LOG_ERROR("create socket error %d", errno);
    }
}

ServerSocket::~ServerSocket()
{
    ::close(fd_);
}

Status ServerSocket::bind(const InetSocketAddress &addr)
{
    int ret = ::bind(fd_, addr.sockaddr_cast(), sizeof(sockaddr_in6));
    if (ret < 0) {
        LOG_ERROR("bind InetSocketAddress error %s:%d", addr.get_address().to_string().c_str(), addr.port());
    }

    listen();

    return ret < 0 ? Status::invalid_argument("invalid address") : Status::ok();
}

void ServerSocket::listen()
{
    if (::listen(fd_, SOMAXCONN) < 0) {
        LOG_ERROR("listen error")
    }
}

int ServerSocket::accept(InetSocketAddress &addr)
{
    uint32_t len = sizeof(addr.sockaddr6_);
    int fd = ::accept4(fd_, addr.sockaddr_cast(), &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (fd < 0) {
        LOG_ERROR("accept4 error %d", errno);
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