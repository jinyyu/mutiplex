#ifndef NET4CXX_UTILS_H
#define NET4CXX_UTILS_H

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>

namespace reactor
{

class MutexLockGuard
{
public:
    explicit MutexLockGuard(pthread_mutex_t* mutex)
        : mutex_(mutex)
    {
        if (pthread_mutex_lock(mutex) != 0) {
            fprintf(stderr, "pthread_mutex_lock error");
            exit(0);
        }
    }

    ~MutexLockGuard()
    {
        if (pthread_mutex_unlock(mutex_) != 0) {
            fprintf(stderr, "pthread_mutex_unlock error");
            exit(0);
        }
    }

private:
    pthread_mutex_t* mutex_;
};

bool is_pow_of_two(uint32_t v);

uint32_t roundup_pow_of_two(uint32_t v);

}

#endif