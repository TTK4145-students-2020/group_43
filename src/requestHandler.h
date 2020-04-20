#pragma once

#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <limits>

#include "con_load.h"
#include "elevator.h"
#include "elevator_io_types.h"
#include "globals.h"

#define TRAVEL_TIME_BETWEEN_FLOOR  2

void requestHandler_updateOtherElevators(elevator_data_t newElevState);
void requestHandler_clearAllHallwayRequests(elevator_data_t* elevator);
void requestHandler_handleDeadElevators();
elevator_data_t* requestHandler_getElevatorBackup(int elevId);
elevator_data_t* requestHandler_getOtherElevators(void);

int requestHandler_toTakeAssignedRequest(request_data_t assignedRequest);
request_data_t requestHandler_assignNewRequest(elevator_data_t* elevator, int btn_floor, Button btn_type);

