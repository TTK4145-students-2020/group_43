#include "threadTimer.hpp"

threadTimer::threadTimer(double duration)
    :   implementation(threadTimerImplementation(duration)) 
{
    implementation.startTimerThread();
}

int threadTimer::isTimedOut()
{
    return implementation.getTimeout();
}

int threadTimer::stopTimer(){
    return implementation.killTimerThread();
}
void threadTimer::resetTimer(){
    implementation.resetTimer();
    return;
}

threadTimer::~threadTimer() {
    implementation.~threadTimerImplementation();
    return;
}
