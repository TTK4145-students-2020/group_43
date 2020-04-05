#pragma once

#include <stdint.h>

#define NUMBER_ELEVATOR 3
#define N_FLOORS 4
#define N_BUTTONS 3


#define COMPUTER_9_IP (char*) "10.100.23.223"
#define COMM_PORT 4300 //Port to receive and send messages with the other elevators
#define LENGHT_MESSAGE 500 // the max length is theoreticaly 512, but it does not work with me. can be reduced for gainz to sizeof(order_data)
#define ID_ASK_RECOVER 200
#define POS_ID_MESSAGE 0
#define NUMBER_MESSAGES 3 //number of messages to send for redudancy
#define TIMEOUT_RECEIVE_MESSAGE 0.01 //seconds
#define ERROR_INCONSITANT_MESSAGE 255

static uint8_t ID_ELEVATOR;

typedef enum {
    EB_Idle,
    EB_DoorOpen,
    EB_Moving
} ElevatorBehaviour_t;


typedef struct 
{
    uint8_t     floor;
	uint8_t 	button; //-1 for going down, 0 for inside button and 1 for going up
	int8_t      owner; //who is taking the order? -1 if nobody
}order_data_t;

typedef struct
{
	uint8_t		floor; //floor where the elevator actually is
	#ifdef cpluplus
	_Bool 		direction; //false = down, true = up
	#else 
	bool		direction;
	#endif
	int8_t		requests[N_FLOORS][N_BUTTONS]; //todo, check if it can be bools, and if it takes less bytes to have it as bool
	ElevatorBehaviour_t 	behaviour; //from the fsm
}elevator_data_t;

typedef struct
{
	order_data_t	order;
	elevator_data_t	elevator;
}message_t;