#pragma once

#include "elevator.h"
#include "elevator_io_types.h"
#include "globals.hpp"

int requestHandler_toTakeAssignedRequest(Elevator elevator, order_data_t assignedRequest);
order_data_t requestHandler_assignNewRequest(Elevator elevator, Elevator otherElevators[], int btn_floor, Button btn_type);


