#include "Network.h"
#include <stdio.h>
#include <unistd.h>

void order_update_queue(order_data_t* received_order)
{
    printf("Order ID number %u received\n", received_order->id);
	printf("is it going here\n");
}

int main(int argc,char** argv)
{
	#ifdef __cplusplus
	printf("considering C++ code\n");
	#else
	printf("considering C code\n");
	#endif
	uint16_t client_port = CLIENT_PORT;
	uint16_t server_port = SERVER_PORT;
	
	if(argc > 1)
	{
		printf("the number of first parameter is %d",(int) (argv[1][0])-'0');
		if(argv[1][0] == '1' ) //configuration 1
		{
			printf("Other config \n");
			uint16_t temp  = client_port;
			client_port = server_port;
			server_port = temp;
		}
	}
	else
	{
		printf("please add a parameter when calling the program.\n");
		printf("To realise a proper test of the network module, \n");
		printf("Start the program twice with 0 and 1 as parameter\n");
		return 0;
	}
	
	
	network_init(client_port);
	
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
        printf("send message ...\n");
        network_broadcast_message(&order, server_port);
        sleep(2);
    }
    return 0;
}
