#include "evcpp/InetAddress.h"
#include "evcpp/Status.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include "Debug.h"

namespace ev
{

InetAddress InetAddress::get_by_address(const char* addr, int family, Status& status)
{
    InetAddress address(family);
    if (inet_pton(family, addr, &address.addr6_) != 1) {
        status = Status::invalid_argument("invalid address");
    }
    return address;
}

InetAddress InetAddress::any(int family)
{
    InetAddress address(family);
    switch (family) {
        case AF_INET: {
            int any = htonl(INADDR_ANY);
            memcpy(&address.addr_, &any, sizeof(any));
            break;
        }
        case AF_INET6: {
            address.addr6_ = in6addr_any;
            break;
        }
        default:
            assert(false);
    }

    return address;
}

InetAddress InetAddress::get_by_host(const char* hostname, Status& status)
{
    InetAddress addr(AF_INET);
    char buf[8192];
    struct hostent hent;
    struct hostent* he = NULL;
    int herrno = 0;
    bzero(&hent, sizeof(hent));

    //only for ip v4
    int ret = gethostbyname_r(hostname, &hent, buf, sizeof(buf), &he, &herrno);
    if (ret == 0 && he != NULL) {
        memcpy(&addr.addr_, he->h_addr, sizeof(addr.addr_));
        status = Status::ok();
    }
    else {
        status = Status::invalid_argument("invalid hostname");
    }
    return addr;
}

std::string InetAddress::to_string() const
{
    char str[INET6_ADDRSTRLEN];
    int af = v4() ? AF_INET : AF_INET6;

    if (inet_ntop(af, &addr_, str, INET6_ADDRSTRLEN) == NULL) {
        LOG_DEBUG("invalid addr");
    }
    return str;
}

bool InetAddress::operator==(const InetAddress& address)
{
    if (this->famliy_ != address.famliy_) {
        return false;
    }

    if (v4()) {
        return memcmp(&addr_, &address.addr_, sizeof(addr_)) == 0;
    }
    else {
        return memcmp(&addr6_, &address.addr6_, sizeof(addr6_)) == 0;
    }
}

bool InetAddress::operator!=(const InetAddress& address)
{
    return !InetAddress::operator==(address);
}

}
