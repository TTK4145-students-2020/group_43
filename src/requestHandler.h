#pragma once

#include "elevator.h"
#include "elevator_io_types.h"
#include "globals.hpp"

int requestHandler_toTakeAssignedRequest(elevator_data_t elevator, order_data_t assignedRequest);
order_data_t requestHandler_assignNewRequest(elevator_data_t elevator, elevator_data_t otherElevators[], int btn_floor, Button btn_type);


