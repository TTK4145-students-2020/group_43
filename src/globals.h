#pragma once

#include <stdint.h>
#include "threadTimer.h"

#define N_ELEVATORS 3
#define N_FLOORS 4
#define N_BUTTONS 3
#define ELEVATOR_TIMEOUT_DURATION 4     //seconds
#define UNINITIALIZED_FLOOR -1

#define RECOVERY_TIMEOUT_DURATION 1 //float in seconds

extern uint8_t ID_ELEVATOR;

typedef enum {
    // Assume everyone waiting for the elevator gets on the elevator, even if 
    // they will be traveling in the "wrong" direction for a while
    CV_All,
    
    // Assume that only those that want to travel in the current direction 
    // enter the elevator, and keep waiting outside otherwise
    CV_InDirn,
} ClearRequestVariant;

typedef struct {
        ClearRequestVariant clearRequestVariant;
        double   			doorOpenDuration_s;
} config_t; 

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
    B_HallUp	= 0,
    B_HallDown	= 1,
    B_Cab		= 2
} Button;

typedef struct 
{
    int8_t      floor;
	Button 	    button; 
	int8_t      ownerId;
}request_data_t;

typedef struct {
	int8_t		id;
	int8_t		floor;
	Dirn 		dirn;
	uint8_t		requests[N_FLOORS][N_BUTTONS];
	ElevatorBehaviour_t 	behaviour;
	threadTimer*            timer;
    config_t*	config;
}elevator_data_t;