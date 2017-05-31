#include "Logger.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <utils.h>

namespace net {


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
  if (fd_ == 2 || buffer_.size() > 1024) {
    flush();
  }
}

void Logger::log(int level, const char* format, ...)
{
  if (level < level_) {
    return;
  }

  va_list args;
  char data[1024];
  va_start(args, format);
  int len = vsprintf(data, format, args);
  append(data, len);
  va_end(args);
}

void Logger::flush()
{
  int remaining = buffer_.size();
  for (size_t i = 0; remaining > 0; ) {
    ssize_t n = ::write(fd_, buffer_.data() + i, remaining);
    if (n < 0) {
      fprintf(stderr, "write error %d\n", errno);
    }
    else if (n == 0) {
      fprintf(stderr, "n == 0");
    }
    else {
      i += n;
      remaining -= n;
    }
  }
  buffer_.clear();
}



static Logger _logger_singleton(2); //stderr

Status set_log_destination(const char *path)
{
  return _logger_singleton.open(path);
}

void set_log_level(int level)
{
  _logger_singleton.set_level(level);
}

Logger* get_logger_singleton()
{
  return &_logger_singleton;
}



}



