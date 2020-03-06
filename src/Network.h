#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "sverresnetwork.h"

#define COMPUTER_9_IP (char*) "10.100.23.223"
#define SERVER_PORT 20010
#define CLIENT_PORT 20010
#define LENGHT_MESSAGE 500 // the max length is theoreticaly 512, but it does not work with me.

typedef union
{
    struct
    {
        char msg[60];
        int32_t a;
        int32_t b;
        int32_t c;
        int32_t d;
        float f;
    }data;

    char message[LENGHT_MESSAGE];
}network_message_t;

