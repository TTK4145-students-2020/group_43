#include "globals.hpp" // might delete when merging
#include "order_timer_implementation.hpp"

/*  Timing functionallity. ( by using threads. )*/
class order_timer 
{
private:
    order_timer_implementation implementation;
public:

    order_timer(order* order_ptr, double duration); /* Starts a timer which sets a timeout-flag after duration [s]*/
    int is_timed_out(); //Returns the timeout-flag; is 1 when timed out, 0 when not
    int stop_timer(); //Stops timer. Returns 0 if successful. (Frees up the processor)

    ~order_timer();
};