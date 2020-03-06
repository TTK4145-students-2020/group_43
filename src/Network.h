#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "sverresnetwork.h"

#define COMPUTER_9_IP (char*) "10.100.23.223"
#define SERVER_PORT 20010 //Port where we send messages
#define CLIENT_PORT 20010 //Port where we receive messages from
#define LENGHT_MESSAGE 500 // the max length is theoreticaly 512, but it does not work with me. can be reduced for gainz to sizeof(order_data)

typedef union
{
    struct
    {
        uint8_t     id; //include the order of the order (what order did we received first)
        uint8_t     floor;
        bool        direction;
        uint32_t    recpetion_time;
        int8_t      owner; //who is taking the order? -1 if nobody
        //uint32_t  take_time; //should be useless as we have the timout module
        uint8_t     destination_floor;
    }order_data;

    char message[LENGHT_MESSAGE];
}network_message_t;

void network_boadcast_message(network_message_t* msg);
void network_init();