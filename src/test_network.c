#include "Network.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc,char** argv)
{
	printf("size of order = %lu vs uint8_t = %lu\n",sizeof(order_data_t),sizeof(uint8_t));
    
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
        printf("broadcast message %d times for redundancy...\n",NUMBER_MESSAGES);
		network_broadcastMessage(&order); //should be server_port
        sleep(2);
    }
    return 0;
}
