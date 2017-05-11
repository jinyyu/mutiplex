#ifndef NET_UTILS_H
#define NET_UTILS_H

#include <pthread.h>
#include <stdio.h>
#include <errno.h>

namespace net
{

class MutexLockGuard
{
 public:
  explicit MutexLockGuard(pthread_mutex_t *mutex)
      : mutex_(mutex) {
    if (pthread_mutex_lock(mutex) != 0) {
      fprintf(stderr, "pthread_mutex_init error %d\n", errno);
    }
  }

  ~MutexLockGuard() {
    pthread_mutex_unlock(mutex_);
  }

 private:
  pthread_mutex_t *mutex_;
};

}

#endif