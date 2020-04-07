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
    return implementation.stopThread();
}
int threadTimer::isAlive(){
    return implementation.isThreadRunning();
}
double threadTimer::getTime(){
    return implementation.getTime();
}
threadTimer::~threadTimer() {
    implementation.~threadTimerImplementation();
    return;
}
