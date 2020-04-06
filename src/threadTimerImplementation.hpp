#pragma once
#include <pthread.h>



class threadTimerImplementation
{
private:
    pthread_mutex_t timerMutex;
    pthread_t timerThread;
    double duration;
    double startTime;
    bool timeout;
    bool threadRunning;
public:
    threadTimerImplementation(double duration);
    ~threadTimerImplementation();

    int startTimerThread();
    bool getTimeout();
    void* timerWork();
    int killTimerThread(); //returns 0 if successful
    void resetTimer();
    int isThreadRunning();
    double getTime();
};
