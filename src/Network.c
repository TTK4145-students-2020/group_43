#include "Network.h"

order_data_t received_order;


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
    order_update_queue(received_order);
}

void network_init()
{	
    printf("Init the network ...\n");
	udp_startReceiving(COMM_PORT, network_receive_message);
 	sleep(2);
	printf("Init the network ... DONE\n");
}

	
void network_broadcast_message(order_data_t* order)
{
    char msg[LENGHT_MESSAGE];
    memcpy(msg, order, LENGHT_MESSAGE); //convert the order struct into sendable char*
    udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); //+2 is not needed, be careful of the termination though
}