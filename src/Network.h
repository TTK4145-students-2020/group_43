#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h> //for sleep function
#include <string.h> //for memcpy
#include "sverresnetwork.h"
#include "Order_handler.h" //to give the received order into the order_handler module
#include "threadTimer.hpp"
#include "fsm.h"
#include "globals.hpp"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define COMM_PORT 4300 //Port to receive and send messages with the other elevators
#define LENGHT_MESSAGE sizeof(message_t) // the max length is theoreticaly 512, but it does not work with me. can be reduced for gainz to sizeof(order_data)

#define NUMBER_MESSAGES 3 //number of messages to send for redudancy
#define SIZE_BUFFER_MESSAGES 2*NUMBER_ELEVATOR //number of places we can store messages in case we receive different messages in the same time (error or not).
#define TIMEOUT_RECEIVE_MESSAGE 0.01 //seconds

#define ERROR_INCONSITANT_MESSAGE 255

#define ID_ASK_RECOVER 200
#define ID_ORDER_MESSAGE 1
#define ID_ELEVATOR_MESSAGE 2

typedef struct //this is the struct that is sent through the network
{
	uint8_t id;
	union 
	{
		order_data_t	order;
		elevator_data_t elevator;
		uint8_t			IdToRecover;
	}data;
}message_t;

void network_init(uint8_t probaErr);
void network_broadcast(order_data_t* msg);
void network_broadcast(elevator_data_t* msg);
void network_askRecovery();

#endif // !#define NETWORK_H
