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

#define NUMBER_ELEVATOR 2

void network_init();
//void network_broadcast_message(order_data_t* msg);
void network_broadcast_message(order_data_t* msg);

#endif // !#define NETWORK_H
