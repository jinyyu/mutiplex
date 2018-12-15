#pragma once
#include <stdint.h>
#include <string>

namespace muti
{

class InetAddress
{
public:
    explicit InetAddress()
        : port_(0),
          ip_(0)
    {

    }

    explicit InetAddress(uint32_t ip, uint16_t port)
        : port_(port),
          ip_(ip)
    {

    }

    // 127.0.0.1:8080
    explicit InetAddress(const std::string& addr);

    InetAddress(const InetAddress& b) = default;

    static bool resolve(const char* name, const char* service, InetAddress& addr);

    uint32_t ip() const
    {
        return ip_;
    }

    uint16_t port() const
    {
        return port_;
    }

    // Host Endian
    uint16_t host_port() const;

    std::string ip_str() const;

    std::string to_string() const;

private:

    // Net Endian
    uint16_t port_;
    uint32_t ip_;
};

}
