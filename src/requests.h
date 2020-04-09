#pragma once

#include "elevator.h"
#include "elevator_io_types.h"


Dirn requests_chooseDirection(elevator_data_t e) __attribute__((pure));

int requests_shouldStop(elevator_data_t e) __attribute__((pure));

elevator_data_t requests_clearAtCurrentFloor(elevator_data_t e) __attribute__((pure));

elevator_data_t requests_clearAtCurrentFloor_sim(elevator_data_t e_old, void onClearedRequest(Button b, int floor));
