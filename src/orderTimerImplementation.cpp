#include <stdio.h>
#include <sys/time.h>
#include "orderTimerImplementation.hpp"
#include <unistd.h>

static double get_wall_time(void){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void* wrapTimerWork(void* p){
        ((orderTimerImplementation*)p)->timerWork();
    return NULL;
}
orderTimerImplementation::orderTimerImplementation(order* orderptr, double duration):
    orderptr(orderptr),
    duration(duration),
    timeout(false)
{
    pthread_mutex_init(&timerMutex, NULL);
}


orderTimerImplementation::~orderTimerImplementation()
{
    killTimerThread();
    pthread_join(timerThread, NULL);
    return;
}


void* orderTimerImplementation::timerWork(){
    double endtime = get_wall_time() + duration;
    while (endtime > get_wall_time()){ 
        usleep(1); 
    }
    pthread_mutex_lock(&timerMutex);
    timeout = true;
    pthread_mutex_unlock(&timerMutex);
    return NULL;
}

int orderTimerImplementation::startTimerThread()
{
    pthread_create(&timerThread, NULL, &wrapTimerWork, (void*)this);   
    return 1;
}


bool orderTimerImplementation::getTimeout(){
    return timeout;
}

int orderTimerImplementation::killTimerThread(){
    return pthread_cancel(timerThread);
}

void orderTimerImplementation::resetTimer(){
    killTimerThread();
    pthread_join(timerThread, NULL);
    pthread_mutex_lock(&timerMutex);
    timeout = false;
    pthread_mutex_unlock(&timerMutex);
    startTimerThread();
    return;
}