#pragma once
#include "globals.hpp" //might delete when merging
#include <pthread.h>



class order_timer_implementation
{
private:
    pthread_mutex_t timer_mutex;
    pthread_t timer_thread;
    double duration;
    order* order_ptr;
    bool timeout;
public:
    order_timer_implementation(order* order_ptr, double duration);
    ~order_timer_implementation();

    int start_timer_thread();
    bool get_timeout();
    void* timer_work();
    int kill_timer_thread(); //returns 0 if successful
};
