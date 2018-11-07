#include "mutiplex/InetAddress.h"
#include "mutiplex/Status.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include <stdexcept>
#include "Debug.h"

namespace muti
{

InetAddress::InetAddress(const std::string& addr)
{
    size_t p = addr.find(':');
    if (p == std::string::npos) {
        throw std::runtime_error("invalid addr");
    }
    std::string ip = addr.substr(0, p);
    std::string port = addr.substr(p + 1);
    if (inet_pton(AF_INET, ip.c_str(), &ip_) != 1) {
        LOG_DEBUG("inet_pton error %s", strerror(errno));
        throw std::runtime_error("invalid ip");
    }


    port_ = htons(static_cast<uint16_t>(std::atoi(port.c_str())));
}

std::string InetAddress::to_string() const
{
    char str[32];
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip_, ipStr, INET_ADDRSTRLEN);
    snprintf(str, 32, "%s:%d", ipStr, host_port());
    return str;
}

std::string InetAddress::ip_str() const
{
    char str[INET_ADDRSTRLEN];
    return inet_ntop(AF_INET, &ip_, str, INET_ADDRSTRLEN);
}

uint16_t InetAddress::host_port() const
{
    return ntohs(port_);
}

}
