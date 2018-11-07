#pragma once

#include <inttypes.h>
#include <string>

namespace muti
{

class Timestamp
{
public:
    static uint64_t current();

    //Formats a timestamp. yyyy-mm-dd hh:mm:ss.ffffff, where fffffff indicates microseconds.
    static std::string to_string(uint64_t timestamp);
};

}
