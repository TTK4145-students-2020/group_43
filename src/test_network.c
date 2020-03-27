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
	
	network_init();
	
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
        printf("broadcast message...\n");
        network_broadcast_message(&order); //should be server_port
        sleep(2);
    }
    return 0;
}
