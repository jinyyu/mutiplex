#include "evcpp/ServerSocket.h"
#include "evcpp/InetAddress.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include "Debug.h"


namespace ev
{

ServerSocket::ServerSocket()
    : fd_(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP))
{
    if (fd_ < 0) {
        LOG_DEBUG("create socket error %d", errno);
    }
}

ServerSocket::~ServerSocket()
{
    ::close(fd_);
}

void ServerSocket::bind(const InetAddress& addr)
{
    struct sockaddr_in in_addr;
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = addr.port();
    uint32_t ip = addr.ip();
    memcpy(&in_addr.sin_addr.s_addr, &ip, sizeof(ip));

    int ret = ::bind(fd_, (const sockaddr*) &in_addr, sizeof(in_addr));
    if (ret < 0) {
        LOG_DEBUG("bind error %s", strerror(errno));
        throw std::runtime_error("invalid addr " + addr.to_string());
    }
    listen();
}

void ServerSocket::listen()
{
    if (::listen(fd_, SOMAXCONN) < 0) {
        LOG_DEBUG("lisetn error %d", errno);
    }
}

int ServerSocket::accept(InetAddress& addr)
{
    struct sockaddr_in in_addr;
    socklen_t len = sizeof(in_addr);
    memset(&in_addr, 0, len);

    int fd = ::accept4(fd_, (struct sockaddr*) &in_addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (fd < 0) {
        LOG_DEBUG("accept4 error %d", errno);
    }
    addr = InetAddress(in_addr.sin_addr.s_addr, in_addr.sin_port);

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