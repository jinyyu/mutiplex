#ifndef NET4CXX_LOGGER_H
#define NET4CXX_LOGGER_H

#include <net4cxx/Status.h>
#include "NonCopyable.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <pthread.h>

namespace net4cxx
{

class Logger: NonCopyable
{

public:
    enum
    {
        INFO = 0,
        WARNING = 1,
        ERROR = 2
    };

    explicit Logger(int fd)
        : fd_(fd), level_(0)
    { pthread_mutex_init(&mutex_, NULL); }

    Status open(const char *file);

    void log(int level, const char *format, ...);

    ~Logger();

    void set_level(int level)
    { level_ = level; }

private:

    void append(char *data, uint32_t len);

    void flush();

private:

    int fd_;
    std::vector<char> buffer_;
    pthread_mutex_t mutex_;
    int level_;

};

Logger *get_logger_singleton();

Status set_log_destination(const char *path);

void set_log_level(int level);

#ifdef DEBUG
#define LOG_DEBUG(format, ...) do { net4cxx::get_logger_singleton()->log(net4cxx::Logger::INFO, "[INFO] [%s:%d] " format "\n", strrchr(__FILE__, '/') + 1, __LINE__, ##__VA_ARGS__); } while(0);
#else
#define LOG_DEBUG(format, ...)
#endif

#define LOG_INFO(format, ...) do { net4cxx::get_logger_singleton()->log(net4cxx::Logger::INFO, "[INFO] [%s:%d] " format "\n", strrchr(__FILE__, '/') + 1, __LINE__, ##__VA_ARGS__); } while(0);
#define LOG_WARNING(format, ...) do { net4cxx::get_logger_singleton()->log(net4cxx::Logger::WARNING, "[WARNING] [%s:%d] " format "\n", strrchr(__FILE__, '/') + 1, __LINE__, ##__VA_ARGS__); } while(0);
#define LOG_ERROR(format, ...) do { net4cxx::get_logger_singleton()->log(net4cxx::Logger::ERROR, "[ERROR] [%s:%d] " format "\n", strrchr(__FILE__, '/') + 1, __LINE__, ##__VA_ARGS__); } while(0);
}

#endif //NET_LOGGER_H
