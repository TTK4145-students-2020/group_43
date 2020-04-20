#pragma once
#include "threadTimerImplementation.h"

/*  Timing functionallity. ( by using threads. Total overkill by the way.)*/
class threadTimer 
{
private:
    threadTimerImplementation implementation;
public:
    threadTimer();      //Default duration = 1 second.
    threadTimer(double duration); /* Starts a timer which sets a timeout-flag after duration [s]*/
    
    int isTimedOut(); //Returns the timeout-flag; is 1 when timed out, 0 when not
    void start(); //Starts timer. Will restart timer if already running.
    int stop(); //Stops timer. Returns 0 if successful. (Frees up the processor)
    int isAlive();  //Returns 1 if the timer thread is currently running
    double getTime(); // Returns the timers current elapsed duration

    ~threadTimer(); //Stops timer thread
};