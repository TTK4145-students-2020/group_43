#pragma once

#include "elevator.h"
#include "elevator_io_types.h"
#include "globals.hpp"

void requestHandler_updateOtherElevators(elevator_data_t newElevState);
elevator_data_t requestHandler_getElevatorBackup(int elevId);

int requestHandler_toTakeAssignedRequest(order_data_t assignedRequest);
order_data_t requestHandler_assignNewRequest(elevator_data_t elevator, int btn_floor, Button btn_type);


