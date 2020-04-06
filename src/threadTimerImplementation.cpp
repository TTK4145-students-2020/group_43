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
    startTime(-1.0),
    timeout(false),
    threadRunning(false)
{
    pthread_mutex_init(&timerMtx, NULL);
    pthread_mutex_init(&startTimeMtx, NULL);
    pthread_mutex_init(&runningMtx, NULL);
}

threadTimerImplementation::~threadTimerImplementation() // This is probably completely unnecessary
{
    stopThread();
    return;
}

void* threadTimerImplementation::timerWork(){
    pthread_mutex_lock(&startTimeMtx);
    startTime = get_wall_time();
    double endtime = startTime + duration;
    pthread_mutex_unlock(&startTimeMtx);
    while (endtime > get_wall_time()){ 
        usleep(1); 
    }
    pthread_mutex_lock(&timerMtx);
    timeout = true;
    pthread_mutex_unlock(&timerMtx);
    pthread_mutex_lock(&runningMtx);
    threadRunning = false;
    pthread_mutex_unlock(&runningMtx);
    return NULL;
}

int threadTimerImplementation::startTimerThread()
{
    
    pthread_create(&timerThread, NULL, &wrapTimerWork, (void*)this);
    pthread_mutex_lock(&runningMtx);
    threadRunning = true;
    pthread_mutex_unlock(&runningMtx);
    
    return 1;
}

bool threadTimerImplementation::getTimeout(){
    return timeout;
}

int threadTimerImplementation::killTimerThread(){
    int ans = !(pthread_cancel(timerThread));
    return ans;
}
int threadTimerImplementation::stopThread(){
    pthread_mutex_lock(&runningMtx);
    bool temp = threadRunning;
    pthread_mutex_unlock(&runningMtx);
    if (temp){
        bool ans = killTimerThread();
        pthread_join(timerThread, NULL);
        pthread_mutex_lock(&runningMtx);
        threadRunning = false;
        pthread_mutex_unlock(&runningMtx);
        return ans;
    }
    else
        return 1;
}

void threadTimerImplementation::resetTimer(){
    stopThread();
    pthread_mutex_lock(&timerMtx);
    timeout = false;
    pthread_mutex_unlock(&timerMtx);
    startTimerThread();
    return;
}

int threadTimerImplementation::isThreadRunning(){
    pthread_mutex_lock(&runningMtx);
    int temp = (int)threadRunning;
    pthread_mutex_unlock(&runningMtx);
    return temp;
}

double threadTimerImplementation::getTime(){
    pthread_mutex_lock(&startTimeMtx);
    double ans = get_wall_time() - startTime;
    pthread_mutex_unlock(&startTimeMtx);
    return ans;
}

