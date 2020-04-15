#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "con_load.h"
#include "elevator_io_types.h"
#include "elevator.h"
#include "costFunc.h"
#include "globals.hpp"

#include "requestHandler.h"

static elevator_data_t      otherElevators[NUMBER_ELEVATOR-1];

static void __attribute__((constructor)) requestHandler_init(){
    for (int i = 0; i<NUMBER_ELEVATOR-1; i++) {
        otherElevators[i] = elevator_uninitialized();
        con_load("elevator.con",
			con_val("doorOpenDuration_s", &otherElevators[i].config->doorOpenDuration_s, "%lf")
			con_enum("clearRequestVariant", &otherElevators[i].config->clearRequestVariant,
				con_match(CV_All)
				con_match(CV_InDirn)
			)
		)
        otherElevators[i].id = -1;
    }
    
}

elevator_data_t* requestHandler_getElevatorBackup(int elevId) {
    for(int i = 0; i<NUMBER_ELEVATOR; i++){
        if(otherElevators[i].id == elevId) {
            return &otherElevators[i];
        }
    }
}

void requestHandler_updateOtherElevators(elevator_data_t newElevState) {
    //find out where elev with ip/id is stored locally
    int elevIndex = 0; 
    for(int i = 0; i<NUMBER_ELEVATOR; i++){
        if(otherElevators[i].id == -1){ //if this is the first time recieving an update form this elevator
            otherElevators[i].id = newElevState.id;
        }
        if(otherElevators[i].id == newElevState.id) {
            elevIndex = i;
            break;
        }
    }
    //syntax to be changed based on how order_data_t is changed, could probabily be done simpler 
    otherElevators[elevIndex].floor = newElevState.floor;
    otherElevators[elevIndex].dirn = newElevState.dirn;
    for(int f = 0; f<N_FLOORS; f++){
        for(int btn = 0; btn<N_BUTTONS; btn++){
            otherElevators[elevIndex].requests[f][btn] = newElevState.requests[f][btn];
        }
    }
    otherElevators[elevIndex].behaviour = newElevState.behaviour;
    otherElevators[elevIndex].timer->start();
}

int requestHandler_toTakeAssignedRequest(order_data_t assignedRequest) {
    if (assignedRequest.owner == ID_ELEVATOR) {
        return 1;
    }
    return 0;
}

// returns an assigned request, where the id of the chosen elevator is included
order_data_t requestHandler_assignNewRequest(elevator_data_t* elevator, int btn_floor, Button btn_type) {
    printf("managed to enter rH_aR\n");
    printf("%d\n",elevator->floor);
    order_data_t newRequest;
    newRequest.floor = btn_floor;
    newRequest.button = btn_type;

    if (btn_type == B_Cab) {
        newRequest.owner = elevator->id;
        return newRequest;
    }
    
    float cost[NUMBER_ELEVATOR];
    for (int i = 0; i<NUMBER_ELEVATOR; i++) {
        cost[i] = INT_MAX;
    }
    
    cost[0] = costFunc_timeToServeRequest(elevator, btn_type, btn_floor);
    int minCostIndex = 0;
    printf("managed to take cost of other el\n");
    for (int i=1; i<NUMBER_ELEVATOR; i++) {
        if (1){//(otherElevators[i].timer->isTimedOut()) {
            cost[i] = costFunc_timeToServeRequest(&otherElevators[i-1], btn_type, btn_floor);
            if ( cost[i] < cost[i-1] ) {
                minCostIndex = i;
            }
        }
    }
    if (minCostIndex == 0) {
        newRequest.owner = elevator->id;
    }
    else {
        newRequest.owner = otherElevators[minCostIndex-1].id;
    }
    printf("finished assigning\n");
    return newRequest;
}