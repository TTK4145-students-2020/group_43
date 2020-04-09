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

typedef enum { 
    B_HallUp,
    B_HallDown,
    B_Cab
} Button;

typedef struct 
{
    int8_t     floor;
	Button 	button; //-1 for going down, 0 for inside button and 1 for going up
	int8_t      owner; //who is taking the order? -1 if nobody
}order_data_t;

typedef struct
{
	int8_t		id;
	int8_t		floor; //floor where the elevator actually is
	Dirn 		dirn; //false = down, true = up
	uint8_t		requests[N_FLOORS][N_BUTTONS]; //todo, do we want a bitfield to safe place?
	ElevatorBehaviour_t 	behaviour; //from the fsm
}elevator_data_t;