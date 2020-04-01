#pragma once

#include "elevator.h"
#include "elevator_io_types.h"

void requestHandler_handleAssignedRequest(Elevator elevator, Elevator otherElevators[], order_data_t assignedRequest, bool requestFromMyElevator)

order_data_t requestHandler_assignNewRequest(Elevator elevator, Elevator otherElevators[], int btn_floor, Button btn_type);


