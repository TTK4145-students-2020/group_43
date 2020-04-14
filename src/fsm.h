#ifndef FSM_H
#define FSM_H

#include "elevator_io_types.h"
#include "globals.hpp"
#include "Network.h"

void fsm_onInitBetweenFloors(void);
void fsm_onRequestButtonPress(int btn_floor, Button btn_type);
void fsm_onFloorArrival(int newFloor);
void fsm_onDoorTimeout(void);

static void fsm_setAllLights(void);

//void fsm_updateOtherElevators(elevator_data_t newState);
elevator_data_t* fsm_getElevator();
void fsm_initFromBackup(elevator_data_t elevBackup);

#endif // FSM_H