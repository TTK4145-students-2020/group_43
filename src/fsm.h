#ifndef FSM_H
#define FSM_H

#include <stdio.h>

#include "con_load.h"
#include "elevator.h"
#include "elevator_io_device.h"
#include "elevator_io_types.h"
#include "requests.h"
#include "timer.h"
#include "globals.h"
#include "Network.h"

void fsm_onInitBetweenFloors(void);
void fsm_onRequestButtonPress(int btn_floor, Button btn_type);
void fsm_onFloorArrival(int newFloor);
void fsm_onDoorTimeout(void);

void fsm_setAllLights(elevator_data_t otherElevators[]);
//void fsm_setAllLights(void);

//void fsm_updateOtherElevators(elevator_data_t newState);
elevator_data_t* fsm_getElevator();
void fsm_initFromBackup(elevator_data_t elevBackup);

#endif // FSM_H