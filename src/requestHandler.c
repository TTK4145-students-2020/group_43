#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "elevator_io_types.h"
#include "elevator.h"
#include "costFunc.h"
#include "globals.hpp"

#include "requestHandler.h"


int requestHandler_toTakeAssignedRequest(Elevator elevator, order_data_t assignedRequest) {
    if (assignedRequest.owner == elevator.id) {
        return 1;
    }
    return 0;
}

// returns an assigned request, where the id of the chosen elevator is included
order_data_t requestHandler_assignNewRequest(Elevator elevator, Elevator otherElevators[], int btn_floor, Button btn_type) {

    order_data_t newRequest;
    newRequest.floor = btn_floor;
    newRequest.button = btn_type;
    //newRequest.taken = -1;

    if (btn_type == B_Cab) {
        newRequest.owner = elevator.id;
        return newRequest;
    }

    float cost[N_ELEVATORS];
    for (int i = 0; i<N_ELEVATORS; i++) {
        cost[i] = INT_MAX;
    }
    cost[0] = costFunc_timeToServeRequest(elevator, btn_type, btn_floor);
    int minCostIndex = 0;
    for (int i=1; i<N_ELEVATORS; i++) {
        if (otherElevators[i].timer.timedOut()) {
            cost[i] = costFunc_timeToServeRequest(otherElevators[i-1], btn_type, btn_floor);
            if ( cost[i] < cost[i-1] ) {
                minCostIndex = i;
            }
        }
    }
    if (minCostIndex == 0) {
        newRequest.owner = elevator.id;
    }
    else {
        newRequest.owner = otherElevators[minCostIndex-1].id;
    }
    return newRequest;
}