#pragma once
#include <stdint.h>
#include <string>

namespace ev
{

class InetAddress
{
public:
    explicit InetAddress()
        : ip_(0),
          port_(0)
    {

    }

    explicit InetAddress(uint32_t ip, uint16_t port)
        : ip_(ip),
          port_(port)
    {

    }

    // 127.0.0.1:8080
    explicit InetAddress(const std::string& addr);

    InetAddress(const InetAddress& b) = default;

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
