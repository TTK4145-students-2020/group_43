#include <stdio.h>
#include <sys/time.h>
#include "threadTimerImplementation.hpp"
#include <unistd.h>

static double get_wall_time(void){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void* wrapTimerWork(void* p){
        ((threadTimerImplementation*)p)->timerWork();
    return NULL;
}
threadTimerImplementation::threadTimerImplementation(double duration):
    duration(duration),
    timeout(false)
{
    pthread_mutex_init(&timerMutex, NULL);
}


threadTimerImplementation::~threadTimerImplementation()
{
    killTimerThread();
    pthread_join(timerThread, NULL);
    return;
}


void* threadTimerImplementation::timerWork(){
    double endtime = get_wall_time() + duration;
    while (endtime > get_wall_time()){ 
        usleep(1); 
    }
    pthread_mutex_lock(&timerMutex);
    timeout = true;
    pthread_mutex_unlock(&timerMutex);
    return NULL;
}

int threadTimerImplementation::startTimerThread()
{
    pthread_create(&timerThread, NULL, &wrapTimerWork, (void*)this);   
    return 1;
}


bool threadTimerImplementation::getTimeout(){
    return timeout;
}

int threadTimerImplementation::killTimerThread(){
    return pthread_cancel(timerThread);
}

void threadTimerImplementation::resetTimer(){
    killTimerThread();
    pthread_join(timerThread, NULL);
    pthread_mutex_lock(&timerMutex);
    timeout = false;
    pthread_mutex_unlock(&timerMutex);
    startTimerThread();
    return;
}