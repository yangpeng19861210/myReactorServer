#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "Mutex.h"
#include "notCopyable.h"
#include <pthread.h>

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
            if (__builtin_expect(errnum != 0, 0))    \
    __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})


class Condition: public notCopyable{
public:
    explicit Condition(MutexLock& mutex)
        :mutex_(mutex){
        pthread_cond_init(&pcond_, NULL);
    }

    ~Condition(){
        pthread_cond_destroy(&pcond_);
    }

    void wait(){
        MutexLock::UnassignGuard ug(mutex_);
        MCHECK(pthread_cond_wait(&pcond_, mutex_.getMutex()));
    }

    bool waitForSeconds(double seconds);

    void notify(){
        pthread_cond_signal(&pcond_);
    }

    void notifyAll(){
        pthread_cond_broadcast(&pcond_);
    }

private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};

#endif
