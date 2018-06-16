#ifndef NET4CXX_DISTRIBUTION_INETADDRESS_H
#define NET4CXX_DISTRIBUTION_INETADDRESS_H
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>

namespace reactor
{

class Status;

class InetAddress
{

public:
    InetAddress(const InetAddress& address)
    {
        this->famliy_ = address.famliy_;
        addr6_ = address.addr6_;
    }

    bool operator==(const InetAddress& address);

    bool operator!=(const InetAddress& address);

    InetAddress& operator=(const InetAddress& address)
    {
        this->famliy_ = address.famliy_;
        addr6_ = address.addr6_;
        return *this;
    }

    bool v4() const
    { return famliy_ == AF_INET; }

    bool v6() const
    { return famliy_ == AF_INET6; }

    int family() const
    { return famliy_; }

    std::string to_string() const;

    //No name service is checked for the validity of the address
    static InetAddress get_by_address(const char* addr, int family, Status& status);

    static InetAddress any(int family);

    //Determines the IP address of a host, given the host's name
    static InetAddress get_by_host(const char* hostname, Status& status);

private:
    explicit InetAddress(int family)
        : famliy_(family)
    { memset(&addr6_, 0, sizeof(addr6_)); }

private:
    int famliy_;

    union
    {
        struct in_addr addr_;
        struct in6_addr addr6_;
    };

    friend class InetSocketAddress;

};

}

#endif
