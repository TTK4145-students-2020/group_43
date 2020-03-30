#pragma once
#include "globals.hpp" //might delete when merging
#include <pthread.h>



class orderTimerImplementation
{
private:
    pthread_mutex_t timerMutex;
    pthread_t timerThread;
    double duration;
    order* orderptr;
    bool timeout;
public:
    orderTimerImplementation(order* orderptr, double duration);
    ~orderTimerImplementation();

    int startTimerThread();
    bool getTimeout();
    void* timerWork();
    int killTimerThread(); //returns 0 if successful
};
