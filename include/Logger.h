#ifndef NET_LOGGER_H
#define NET_LOGGER_H

#include "Status.h"
#include "NonCopyable.h"
#include <stdio.h>
#include <stdarg.h>
#include <vector>

namespace net
{

class Logger : NonCopyable
{
public:

  explicit Logger(int fd);

  Status open(const char* file);

  void log(const char* format, ...);

  ~Logger();

private:

  void append(char* data, uint32_t len);

  void flush();

private:

  int fd_;
  std::vector<char> buffer_;
  pthread_mutex_t mutex_;

};

Logger* get_logger_singleton();

Status set_log_destination(const char* path);

#define LOG(format, ...) do { net::get_logger_singleton()->log("[%s:%d] " format "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0);


}

#endif //NET_LOGGER_H
