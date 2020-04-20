#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h> //for sleep function
#include <string.h> //for memcpy
#include "sverresnetwork.h"
#include "requestHandler.h"
#include "threadTimer.h"
#include "fsm.h"
#include "globals.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define COMM_PORT 4300 //Port to receive and send messages with the other elevators
#define LENGHT_MESSAGE sizeof(message_t) 

#define N_MESSAGES 4 //number of messages to send for redudancy
#define SIZE_BUFFER_MESSAGES 2*N_ELEVATORS //number of places we can store messages in case we receive different messages in the same time (error or not).
#define TIMEOUT_RECEIVE_MESSAGE 0.01 //seconds

#define ERROR_INCONSITANT_MESSAGE 255

#define ID_ASK_RECOVER 200
#define ID_REQUEST_MESSAGE 1
#define ID_ELEVATOR_MESSAGE 2

#define DEBUG false

typedef struct //this is the struct that is sent through the network
{
	uint8_t id;
	union 
	{
		request_data_t	request;
		elevator_data_t elevator;
		uint8_t			IdToRecover;
	}data;
}message_t;

void network_init();
void network_broadcast(request_data_t* msg);
void network_broadcast(elevator_data_t* msg);
bool network_busyAskRecovery(float timeOut);

#endif // !#define NETWORK_H
