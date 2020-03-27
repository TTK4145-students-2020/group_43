#include "Network.h"

order_data_t received_order;


void order_update_queue_2(order_data_t received_order)
{
    //printf("Order ID number %u received\n", received_order->id);
	printf("is it going here\n");
}


void network_receive_message(const char* ip, char* data, int datalength)
{
	if (datalength !=LENGHT_MESSAGE)
	{
		printf("Unknown message\n");
		return;
	}
    printf("Received UDP message from %s\n", ip);
    memcpy(&received_order, data, datalength); // convert the message in a readable order
    /*printf("if this string is the orderList struct, then we receive these data\n%d\t%d\t%d\t%d\t%f\n",
        receivedMessage.data.a, receivedMessage.data.b, receivedMessage.data.c, receivedMessage.data.d, receivedMessage.data.f);
        */
    order_update_queue_2(received_order);
	printf("end of receive message function\n");
}

void network_init(uint16_t elevatorId)
{	
    printf("Init the network ...\n");
	udp_startReceiving(CLIENT_PORT, network_receive_message);
 /*   //listening to the other elevators
	for (uint8_t i = 1; i<= NUMBER_ELEVATOR;i++) //create a listenning thread for each other elevator
	{
		if (i!=elevatorId)
		{
			udp_startReceiving(CLIENT_PORT+i, network_receive_message);
			printf("Start receiving in port %d ...\n",CLIENT_PORT+i);
		}
	}*/
	sleep(2);
	printf("Init the network ... DONE\n");
}

	
void network_broadcast_message(order_data_t* order, uint16_t port)
{
    char msg[LENGHT_MESSAGE];
    memcpy(msg, order, LENGHT_MESSAGE-10); //convert the order struct into sendable char*
    udp_broadcast(port, msg, LENGHT_MESSAGE); //+2 is not needed, be careful of the termination though
	//udp_broadcast(4321,"Hello all",10);
}

/*
int main() {

    network_init();

    while (1)
    {
        printf("try to send a message\n");
        udp_send(COMPUTER_9_IP, 4320, orderList.message, LENGHT_MESSAGE); //port, data, length=64 in this case
        sleep(2);
        //	udp_send(USING_IP,4321, "send nudes" ,60); //port, data, length
        sleep(2);
    }
    return 0;
}*/
