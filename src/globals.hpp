#pragma once

#include <stdint.h>


#define COMPUTER_9_IP (char*) "10.100.23.223"
#define COMM_PORT 4300 //Port to receive and send messages with the other elevators
#define LENGHT_MESSAGE 500 // the max length is theoreticaly 512, but it does not work with me. can be reduced for gainz to sizeof(order_data)
#define ID_ASK_RECOVER -1
#define POS_ID_MESSAGE 0
#define NUMBER_ELEVATOR 3
#define NUMBER_MESSAGES 3 //number of messages to send for redudancy
#define TIMEOUT_RECEIVE_MESSAGE 1
#define ERROR_INCONSITANT_MESSAGE 255


typedef struct 
{
    int8_t     id; //include the ortder order (what order did we received first)
    uint8_t     floor;
	#ifdef __cplusplus
    bool        direction; //1 for up
	#else
	_Bool		direction; //1 for up
	#endif
    uint32_t    reception_time;
    int8_t      owner; //who is taking the order? -1 if nobody
    uint8_t     destination_floor;
}order_data_t;
