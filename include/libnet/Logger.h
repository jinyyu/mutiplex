#ifndef NET_LOGGER_H
#define NET_LOGGER_H

#include <libnet/Status.h>
#include <libnet/NonCopyable.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>

#include <pthread.h>

namespace net
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

#define LOG_INFO(format, ...) do { net::get_logger_singleton()->log(net::Logger::INFO, "[INFO] [%s:%d] " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0);
#define LOG_WARNING(format, ...) do { net::get_logger_singleton()->log(net::Logger::WARNING, "[WARNING] [%s:%d] " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0);
#define LOG_ERROR(format, ...) do { net::get_logger_singleton()->log(net::Logger::ERROR, "[ERROR] [%s:%d] " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0);
}

#endif //NET_LOGGER_H
