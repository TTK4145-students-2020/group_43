#include "globals.hpp" // might delete when merging
#include "threadTimerImplementation.hpp"

/*  Timing functionallity. ( by using threads. )*/
class threadTimer 
{
private:
    threadTimerImplementation implementation;
public:

    threadTimer(double duration); /* Starts a timer which sets a timeout-flag after duration [s]*/
    int isTimedOut(); //Returns the timeout-flag; is 1 when timed out, 0 when not
    int stopTimer(); //Stops timer. Returns 0 if successful. (Frees up the processor)
    void resetTimer(); //Resets timer

    ~threadTimer(); //Stops timer thread
};