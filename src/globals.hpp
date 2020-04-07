#pragma once

#include <stdint.h>

#define NUMBER_ELEVATOR 3
#define N_FLOORS 4
#define N_BUTTONS 3

//--------------NETWORK-----------
#define COMPUTER_9_IP (char*) "10.100.23.223"

static uint8_t ID_ELEVATOR;

typedef enum {
    EB_Idle,
    EB_DoorOpen,
    EB_Moving
} ElevatorBehaviour_t;

typedef enum { 
    D_Down  = -1,
    D_Stop  = 0,
    D_Up    = 1
} Dirn;

typedef struct 
{
    uint8_t     floor;
	uint8_t 	button; //-1 for going down, 0 for inside button and 1 for going up
	int8_t      owner; //who is taking the order? -1 if nobody
}order_data_t;

typedef struct
{
	uint8_t		id;
	uint8_t		floor; //floor where the elevator actually is
	Dirn 		direction; //false = down, true = up
	int8_t		requests[N_FLOORS][N_BUTTONS]; //todo, check if it can be bools, and if it takes less bytes to have it as bool
	ElevatorBehaviour_t 	behaviour; //from the fsm
}elevator_data_t;