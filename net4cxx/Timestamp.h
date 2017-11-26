#ifndef NET4CXX_DISTRIBUTION_TIMESTAMP_H
#define NET4CXX_DISTRIBUTION_TIMESTAMP_H

#include <inttypes.h>
#include <string>

namespace net4cxx
{

class Timestamp
{
public:
    Timestamp()
        : timestamp_(0)
    {}

    Timestamp(const Timestamp& timestamp)
        : timestamp_(timestamp.timestamp_)
    {}

    //Constructs a Timestamp object using a milliseconds time value
    Timestamp(uint64_t timestamp)
        : timestamp_(timestamp)
    {}

    Timestamp& operator=(const Timestamp& timestamp)
    {
        this->timestamp_ = timestamp.timestamp_;
        return *this;
    }

    bool operator<(const Timestamp& timestamp) const
    { return this->timestamp_ < timestamp.timestamp_; }

    bool operator>(const Timestamp& timestamp) const
    { return this->timestamp_ > timestamp.timestamp_; }

    bool operator==(const Timestamp& timestamp) const
    { return this->timestamp_ == timestamp.timestamp_; }

    bool operator!=(const Timestamp& timestamp) const
    { return this->timestamp_ != timestamp.timestamp_; }

    bool operator<=(const Timestamp& timestamp) const
    { return this->timestamp_ <= timestamp.timestamp_; }

    bool operator>=(const Timestamp& timestamp) const
    { return this->timestamp_ >= timestamp.timestamp_; }

    static Timestamp currentTime();

    //Formats a timestamp. yyyy-mm-dd hh:mm:ss.ffffff, where fffffff indicates microseconds.
    std::string to_string() const;

private:
    uint64_t timestamp_; //microseconds
};

}

#endif //LIBNET_DISTRIBUTION_TIMESTAMP_H
