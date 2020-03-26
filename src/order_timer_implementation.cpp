#include <stdio.h>
#include <sys/time.h>
#include "order_timer_implementation.hpp"

static double get_wall_time(void){
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void* wrap_timer_work(void* p){
        ((order_timer_implementation*)p)->timer_work();
    return NULL;
}
order_timer_implementation::order_timer_implementation(order* order_ptr, double duration):
    order_ptr(order_ptr),
    duration(duration),
    timeout(false)
{
    pthread_mutex_init(&timer_mutex, NULL);
}


order_timer_implementation::~order_timer_implementation()
{

}


void* order_timer_implementation::timer_work(){
    double endtime = get_wall_time() + duration;
    while (endtime > get_wall_time()){ //Do nothing 
    }
    pthread_mutex_lock(&timer_mutex);
    timeout = true;
    pthread_mutex_unlock(&timer_mutex);
    return NULL;
}

int order_timer_implementation::start_timer_thread()
{
    pthread_create(&timer_thread, NULL, &wrap_timer_work, (void*)this);   
    return 1;
}


bool order_timer_implementation::get_timeout(){
    return timeout;
}

int order_timer_implementation::kill_timer_thread(){
    return pthread_cancel(timer_thread);
}