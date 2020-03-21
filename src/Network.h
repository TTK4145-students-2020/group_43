#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "sverresnetwork.h"
#include "Order_handler.h" //to giev the received order into the order_handler module

#define COMPUTER_9_IP (char*) "10.100.23.223"
#define SERVER_PORT 20010 //Port where we send messages
#define CLIENT_PORT 20010 //Port where we receive messages from
#define LENGHT_MESSAGE 500 // the max length is theoreticaly 512, but it does not work with me. can be reduced for gainz to sizeof(order_data)

void network_init();
void network_boadcast_message(order_data_t* msg);

#endif // !#define NETWORK_H
