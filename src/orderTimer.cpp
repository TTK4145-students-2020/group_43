#include "orderTimer.hpp"

orderTimer::orderTimer(order* orderptr, double duration)
    :   implementation(orderTimerImplementation(orderptr, duration)) 
{
    implementation.startTimerThread();
}

int orderTimer::isTimedOut()
{
    return implementation.getTimeout();
}

int orderTimer::stop_timer(){
    return implementation.killTimerThread();
}

orderTimer::~orderTimer() {}
