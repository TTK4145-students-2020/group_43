#include <stdio.h>
#include <stdint.h> //for uint8_t, ...
#include <string.h> //for memcpy
#include <unistd.h>
#include "../src/sverresnetwork.h"

#define LENGHT_MESSAGE 500 // the max length is theoreticaly 512, but it does not work with me. can be reduced for gainz to sizeof(order_data)

typedef struct 
{
    uint8_t     id; //include the ortder order (what order did we received first)
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


void udpmessageReceived(const char * ip, char * data, int datalength){

  // Assuming an ascii string here - a binary blob (including '0's) will
  // be ugly/truncated.
  printf("Received UDP message from %s: '%s'\n",ip,data);
  
}

int main(){
	
	char msg[LENGHT_MESSAGE];
	order_data_t order;
    int i = 42;
  udp_startReceiving(4321,udpmessageReceived);
	while(1) 
	{
		order.id = i++;
		memcpy(msg, &order, LENGHT_MESSAGE-10); //convert the order struct into sendable char*
		udp_broadcast(4321,msg,LENGHT_MESSAGE);
		sleep(1);
	}
  return 0;
}

