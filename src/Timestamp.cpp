#include "mutiplex/Timestamp.h"
#include <sys/time.h>


namespace muti
{

#define MICROSECONDS_PER_SECOND  (1000 * 1000)

uint64_t Timestamp::current()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<uint64_t>(tv.tv_sec * MICROSECONDS_PER_SECOND + tv.tv_usec);
}

std::string Timestamp::to_string(uint64_t timestamp)
{
    struct timeval tm;
    tm.tv_sec = timestamp / MICROSECONDS_PER_SECOND;
    tm.tv_usec = timestamp % MICROSECONDS_PER_SECOND;

    struct tm result;
    localtime_r(&tm.tv_sec, &result);

    char buf[64];

    snprintf(buf,
             sizeof(buf),
             "%04d-%02d-%02d %02d:%02d:%02d.%06lu",
             result.tm_year + 1900,
             result.tm_mon + 1,
             result.tm_mday,
             result.tm_hour,
             result.tm_min,
             result.tm_sec,
             tm.tv_usec);
    return buf;
}

}