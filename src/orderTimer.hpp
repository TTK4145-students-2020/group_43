#include "globals.hpp" // might delete when merging
#include "orderTimerImplementation.hpp"

/*  Timing functionallity. ( by using threads. )*/
class orderTimer 
{
private:
    orderTimerImplementation implementation;
public:

    orderTimer(order* orderptr, double duration); /* Starts a timer which sets a timeout-flag after duration [s]*/
    int isTimedOut(); //Returns the timeout-flag; is 1 when timed out, 0 when not
    int stopTimer(); //Stops timer. Returns 0 if successful. (Frees up the processor)
    void resetTimer(); //Resets timer

    ~orderTimer(); //Stops timer thread
};