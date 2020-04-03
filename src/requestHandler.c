#include <limits.h>
#include <string.h>

#include "elevator_io_types.h"
#include "costFunc.h"
#include "Network.h"
// #include order_timer____.h

#include "requestHandler.h"

/*
void requestHandler_handleAssignedRequest(Elevator elevator, Elevator otherElevators[], order_data_t assignedRequest, bool requestFromMyElevator) {
    if (assignedRequest.taken == -1) {
        if (assignedRequest.ownerId == elevator.id) {
            assignedRequest.taken = 1;
            network_broadcast_message(&assignedRequest);
            fsm_onRequestButtonPress(assignedRequest.btn_floor, assignedRequest.btn_type);
        }
        elseif(requestFromMyElevator) {
            network_broadcast_message(&assignedRequest);
        }
    }
    elseif (assignedRequest.taken == 1) {
        for (int i = 0; i<N_ELEVAORS-1; i++) {
            if (otherElevators[i].id == assignedRequest.ownerId) {
                otherElevators[i].requests[assignedRequest.btn_floor][assignedRequest.btn_type] = 1;
                //start timerThread for other elevator which has ownerId if that elevator is Idle (has no timer thread)
                if (otherElevators[i].behaviour == EB_Idle)
                    otherElevators[i].timer.resetTimer();
            }
        }
        
    }   
}
*/

bool requestHandler_toTakeAssignedRequest(Elevator elevator, order_data_t assignedRequest) {
    if (assignedRequest.owner == elevator.id) {
        return true;
    }
    return false;
}

// returns an assigned request, where the id of the chosen elevator is included
order_data_t requestHandler_assignNewRequest(Elevator elevator, Elevator otherElevators[], int btn_floor, Button btn_type) {

    order_data_t newRequest;
    newRequest.btn_floor = btn_floor;
    newRequest.btn_type = btn_type;
    newRequest.taken = -1;

    if (btn_type == B_Cab) {
        newRequest.ownerId = myId;
        return newRequest;
    }

    int cost[N_ELEVATORS]
    for (int i = 0; i<N_ELEVATORS; i++) {
        cost[i] = INT_MAX;
    }
    cost[0] = costFunc_timeToServeRequest(elevator, btn_type, btn_floor);
    int minCostIndex = 0;
    for (int i=0; i<N_ELEVATORS-1; i++) {
        if (otherElevators[i].active) {
            cost[i+1] = costFunc_timeToServeRequest(otherElevators[i], btn_type, btn_floor);
            if ( cost[i] < cost[i-1] ) {
                minCostIndex = i;
            }
        }
    }
    if (minCostIndex == 0) {
        newRequest.ownerId = elevator.id;
    }
    else {
        newRequest.ownerId = elevatorList[minCostIndex].id;
    }
}