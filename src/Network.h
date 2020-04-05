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

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */



void network_init(uint8_t probaErr);
void network_broadcastMessage(order_data_t* order);
void network_askRecovery();

#endif // !#define NETWORK_H
