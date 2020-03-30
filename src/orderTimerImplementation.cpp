#include <stdio.h>
#include <sys/time.h>
#include "orderTimerImplementation.hpp"

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
    order_ptr(order_ptr),
    duration(duration),
    timeout(false)
{
    pthread_mutex_init(&timerMutex, NULL);
}


orderTimerImplementation::~orderTimerImplementation()
{

}


void* orderTimerImplementation::timer_work(){
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
}