#include "Network.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc,char** argv)
{
	#ifdef __cplusplus
	printf("considering C++ code\n");
	#else
	printf("considering C code\n");
	#endif
	uint16_t server_port = SERVER_PORT;
	uint8_t elevatorId;
	if(argc > 1)
	{
		elevatorId = (uint8_t)(argv[1][0]-'0');
		printf("program for elevator number %u out of %d started\n",elevatorId,NUMBER_ELEVATOR);
		server_port += elevatorId;
	}
	else
	{
		printf("please add a parameter when calling the program.\n");
		printf("To realise a proper test of the network module, \n");
		printf("Start the program twice with 0 and 1 as parameter\n");
		return 0;
	}
	
	
	network_init(elevatorId);
	
    order_data_t order;
    
    order.id = 0;
    order.floor = 0;
    order.direction = 1;
    order.owner = -1;
    order.destination_floor = 2;
	
    uint8_t i = 0;
	sleep(2);
    while(1)
    {
        order.id = i++;
        printf("broadcast message to port %d...\n",server_port);
        network_broadcast_message(&order, CLIENT_PORT); //should be server_port
        sleep(2);
    }
    return 0;
}
