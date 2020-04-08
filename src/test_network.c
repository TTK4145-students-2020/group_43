#include "Network.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc,char** argv)
{
	if(argc ==1)
		printf("\nPlease add arguments: \n");
		printf("First one is the elevator ID\n");
		printf("Second one is the probability of error while sending a message\n\n");
				
	ID_ELEVATOR = 1;
	uint8_t probaRandomError = 0;
	if(argc>1)
	{
		ID_ELEVATOR = atoi(argv[1]);
	}
	if(argc>2)
	{
		probaRandomError = atoi(argv[2]);
	}
	printf("proba rand err = %u%%\n",probaRandomError);
	printf("size of elevator_data_t = %lu vs uint8_t = %lu\n",sizeof(elevator_data_t),sizeof(uint8_t));
    
	#ifdef __cplusplus
	printf("considering C++ code\n");
	#else
	printf("considering C code\n");
	#endif
	
	network_init(probaRandomError);
	sleep(2); // it takes some time in another thread
	
    order_data_t order;
	
    order.floor = 0;
    order.button = 1;
    order.owner = -1;
    
	elevator_data_t elevator;
	elevator.floor =1;
	
    uint8_t i = 1;
	sleep(2);
    while(1)
    {
        order.floor = i++;
		elevator.floor =2*i;
        printf("broadcast message %d times for redundancy...\n",NUMBER_MESSAGES);
		//network_broadcastMessage(&order); 
		network_broadcast(&order);
        sleep(2);
		network_broadcast(&elevator);
		sleep(2);
    }
    return 0;
}
