#include <limits.h>

#include "elevator_io_types.h"
#include "cost_func.h"
#include "fsm.h"
#include "Network.h"

#include "request_handler.h"


void request_handler_handle_new_request(int btn_floor, Button btn_type) {
    int cost[N_ELEVATORS]
    cost(0) = cost_func_timeToServeRequest(e_old, btn_type, btn_floor);
    for (int i=1; i<N_ELEVATORS; i++) {
        if (elevator(i) is_alive && btn != B_Cab) {
            cost(i) = network_requestCost(elevator(i).ipNum, cost(0), btn_floor, btn_type);
        }
        else {
            cost(i) = INT_MAX;
        }
    }
    if (min(cost) == 1) {
        fsm_onRequestButtonPress(int btn_floor, Button btn_type);
        comfirm that order is taken to other elevators
    }
    else {
        wait for comfirmation until comfirmation recieved or timeout 
        if timeout: take order yourself and comfirm taking it
    }
}