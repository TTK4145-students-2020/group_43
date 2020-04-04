#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h> //for sleep function
#include <string.h> //for memcpy
#include "sverresnetwork.h"
#include "Order_handler.h" //to give the received order into the order_handler module
#include "order_timer.hpp"
#include "globals.hpp"


void network_init();
//void network_broadcast_message(order_data_t* msg);
void network_broadcastMessage(order_data_t* msg);
void network_askRecovery();

#endif // !#define NETWORK_H
