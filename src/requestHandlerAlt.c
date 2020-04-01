#include <limits.h>
#include <string.h>

#include "elevator_io_types.h"
#include "costFunc.h"
#include "fsm.h"
#include "Network.h"
#include "orderTimer.hpp"
// #include order_timer____.h

#include "requestHandler.h"

// refering to the owner, -1 nontaken order, 0 no order, 1 i am owner, 2 someone else is owner
static int allElevatorRequests[N_FLOORS][N_BUTTONS];

static void requestHandler_setAllElevatorRequestsValue(int btn_floor, Button btn_type, int value) {
    allElevatorRequests[btn_floor][btn_type] = order.value;
}
int requestHandler_getAllElevatorRequestsValue(int btn_floor, Button btn_type) {
    return allElevatorRequests[btn_floor][btn_type];
}
/*  
    - slimmed down order_data_t
    - think we only need 4 variables actually, btn_floor, btn_type, ownerId
    - can't find where original is defined anymore, so defined my version here for now 
*/ 

typedef order_data_t
{
    uint8_t     btn_floor;
    Button      btn_type;
    //uint32_t    recpetion_time;
    //int8_t      taken; // -1 if not taken; 1 for taken; 0 for no order
    int8_t      owner;
};

/* changes to network_receive_message(const char* ip, char* data, int datalength):
    -replace order_update_queue(received_order) with
        requestHandler_setAllElevatorRequestsValue(int btn_floor, Button btn_type, int value)
*/


bool requestHandler_shouldTakeRequest(Elevator elevator, Elevator otherElevators[], int btn_floor, Button btn_type) {

    if (btn_type == B_Cab) {
        return true;
    }

    int cost[N_ELEVATORS]
    for (int i = 0; i<N_ELEVATORS; i++) {
        cost[i] = INT_MAX;
    }
    cost[0] = costFunc_timeToServeRequest(elevator, btn_type, btn_floor);
    int minCostIndex = 0;
    for (int i=1; i<N_ELEVATORS; i++) {
        if (elevatorList[i].active) {
            cost[i] = costFunc_timeToServeRequest(otherElevators[i-1], btn_type, btn_floor);
            if ( cost[i] < cost[i-1] ) {
                minCostIndex = i;
            }
        }
    }
    if (minCostIndex == 0) {
        return true;
    }
    else {
        return false;
    }
}