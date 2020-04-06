#include "threadTimer.hpp"

threadTimer::threadTimer()
    :   implementation(threadTimerImplementation(1)) 
{}
threadTimer::threadTimer(double duration)
    :   implementation(threadTimerImplementation(duration)) 
{}

int threadTimer::isTimedOut()
{
    return implementation.getTimeout();
}
void threadTimer::start(){
    implementation.resetTimer();
    return;
}
int threadTimer::stop(){
    return implementation.killTimerThread();
}
int threadTimer::isAlive(){
    return implementation.isThreadRunning();
}
threadTimer::~threadTimer() {
    implementation.~threadTimerImplementation();
    return;
}
