#include "requestHandler.h"

static elevator_data_t      otherElevators[NUMBER_ELEVATOR-1];
//uint8_t ID_ELEVATOR = 1;

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

elevator_data_t* requestHandler_getOtherElevators(void) {
    return otherElevators;
}

elevator_data_t* requestHandler_getElevatorBackup(int elevId) {
    for(int i = 0; i<NUMBER_ELEVATOR; i++){
        if(otherElevators[i].id == elevId) {
            return &otherElevators[i];
        }
    }
    return NULL;
}

void requestHandler_updateOtherElevators(elevator_data_t newElevState) {
    if(newElevState.id == ID_ELEVATOR) return; //it was not an other elevator but our
	//find out where elev with ip/id is stored locally
    printf("updating other elevator with id=%d\n",newElevState.id);
    int elevIndex = 0; 
    for(int i = 0; i<NUMBER_ELEVATOR; i++){
        if(otherElevators[i].id == -1){ //if this is the first time recieving an update form this elevator
            otherElevators[i].id = newElevState.id;
			printf("new elevator in the network, id %u\n",newElevState.id);
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
    printf("updated other elevator with id=%d, elevIndex = %d  ..", otherElevators[elevIndex].id, elevIndex);
    otherElevators[elevIndex].timer->start();
    elevator_print(otherElevators[elevIndex]); //print of that elevator
}

int requestHandler_toTakeAssignedRequest(order_data_t assignedRequest) {
    if (assignedRequest.owner == ID_ELEVATOR) {
        return 1;
    }
    return 0;
}

// returns an assigned request, where the id of the chosen elevator is included
order_data_t requestHandler_assignNewRequest(elevator_data_t* elevator, int btn_floor, Button btn_type) {
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
    for (int i=1; i<NUMBER_ELEVATOR; i++) {
        printf("checking otherElevator[%d]  \n", i-1); 
        if (!otherElevators[i-1].timer->isTimedOut() && otherElevators[i-1].floor > -1) {  // changed [i] to [i-1].
            printf("floor of other elevator=%d\n",otherElevators[i-1].floor);
            cost[i] = costFunc_timeToServeRequest(&otherElevators[i-1], btn_type, btn_floor);
            if ( cost[i] < cost[i-1] ) {
                printf("rH_ass: other elevator with id=%d\n",otherElevators[i].id);
                minCostIndex = i;
            }
            printf("    .. OK .getTime() = %f \n", otherElevators[i-1].timer->getTime());

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