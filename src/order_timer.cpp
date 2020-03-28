#include "order_timer.hpp"

order_timer::order_timer(order_data_t* order_ptr, double duration)
    :   implementation(order_timer_implementation(order_ptr, duration)) 
{
    implementation.start_timer_thread();
}

int order_timer::is_timed_out()
{
    return implementation.get_timeout();
}

int order_timer::stop_timer(){
    return implementation.kill_timer_thread();
}

order_timer::~order_timer() {}
