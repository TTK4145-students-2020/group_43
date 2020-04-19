#pragma once

#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "con_load.h"
#include "costFunc.h"
#include "elevator.h"
#include "elevator_io_types.h"
#include "globals.h"

void requestHandler_updateOtherElevators(elevator_data_t newElevState);
void requestHandler_wipeHallwayRequests(elevator_data_t* elevator);
elevator_data_t* requestHandler_getElevatorBackup(int elevId);
elevator_data_t* requestHandler_getOtherElevators(void);

int requestHandler_toTakeAssignedRequest(order_data_t assignedRequest);
order_data_t requestHandler_assignNewRequest(elevator_data_t* elevator, int btn_floor, Button btn_type);

