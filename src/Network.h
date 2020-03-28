#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h> //for sleep function
#include <string.h> //for memcpy
#include "sverresnetwork.h"
#include "Order_handler.h" //to give the received order into the order_handler module

#define COMPUTER_9_IP (char*) "10.100.23.223"
#define COMM_PORT 4300 //Port to receive and send messages with the other elevators
#define LENGHT_MESSAGE 500 // the max length is theoreticaly 512, but it does not work with me. can be reduced for gainz to sizeof(order_data)
#define ID_ASK_RECOVER -1
#define POS_ID_MESSAGE 0
#define NUMBER_ELEVATOR 3
#define NUMBER_MESSAGES 3 //number of messages to send for redudancy
#define ERROR_INCONSITANT_MESSAGE 255

void network_init();
//void network_broadcast_message(order_data_t* msg);
void network_broadcastMessage(order_data_t* msg);
void network_askRecovery();

#endif // !#define NETWORK_H
