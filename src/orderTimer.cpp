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

int orderTimer::stopTimer(){
    return implementation.killTimerThread();
}
void orderTimer::resetTimer(){
    implementation.resetTimer();
    return;
}

orderTimer::~orderTimer() {
    implementation.~orderTimerImplementation();
    return;
}
