#include "Logger.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <utils.h>

namespace net {

Logger::Logger(int fd) : fd_(fd)
{
  pthread_mutex_init(&mutex_, NULL);
}

Logger::~Logger()
{
  flush();
  pthread_mutex_destroy(&mutex_);
  ::close(fd_);
}

Status Logger::open(const char *file)
{
  Status status;

  int fd = ::open(file, O_RDWR | O_CLOEXEC | O_APPEND | O_CREAT, S_IRWXU);
  if (fd == -1) {
    printf("%d\n", errno);
    return Status::io_error(__FUNCTION__);
  }


  fd_ = fd;
  return status;
}

void Logger::append(char *data, uint32_t len)
{
  MutexLockGuard lockGuard(&mutex_);
  buffer_.insert(buffer_.end(), data, data + len);
  if (buffer_.size() > 1024) {
    flush();
  }
}

void Logger::log(const char* format, ...)
{
  va_list args;
  char data[1024];
  va_start(args, format);
  int len = vsprintf(data, format, args);
  va_end(args);
  append(data, len);
}

void Logger::flush()
{
  size_t len = ::write(fd_, buffer_.data(), buffer_.size());
  if (len != buffer_.size()) {
    fprintf(stderr, "write error %lu", len);
  }

  buffer_.clear();
}



static Logger _logger_singleton(2); //stderr

Status set_log_destination(const char *path)
{
  return _logger_singleton.open(path);
}

Logger* get_logger_singleton()
{
  return &_logger_singleton;
}

}



