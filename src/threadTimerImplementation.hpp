#pragma once
#include <pthread.h>



class threadTimerImplementation
{
private:
    pthread_mutex_t timerMtx;
    pthread_mutex_t startTimeMtx;
    pthread_mutex_t runningMtx;
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
    int killTimerThread();
    int stopThread();
    void resetTimer();
    int isThreadRunning();
    double getTime();
};
