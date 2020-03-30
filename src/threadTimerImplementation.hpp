#pragma once
#include <pthread.h>



class threadTimerImplementation
{
private:
    pthread_mutex_t timerMutex;
    pthread_t timerThread;
    double duration;
    bool timeout;
public:
    threadTimerImplementation(double duration);
    ~threadTimerImplementation();

    int startTimerThread();
    bool getTimeout();
    void* timerWork();
    int killTimerThread(); //returns 0 if successful
    void resetTimer();
};
