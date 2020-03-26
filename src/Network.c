#include "Network.h"

void network_receive_message(const char* ip, char* data, int datalength)
{
    printf("Received UDP message from %s\n", ip);
    order_data_t received_order;
    memcpy(&received_order, data, LENGHT_MESSAGE); // convert the message in a readable order
    /*printf("if this string is the orderList struct, then we receive these data\n%d\t%d\t%d\t%d\t%f\n",
        receivedMessage.data.a, receivedMessage.data.b, receivedMessage.data.c, receivedMessage.data.d, receivedMessage.data.f);
        */
    order_update_queue(&received_order);
}

void network_init(uint16_t port)
{
    printf("Init the network ...\n");
    //listening to the other elevators
	printf("Start receiving in port %d ...\n",port);
    udp_startReceiving(port, network_receive_message);
}

	
void network_broadcast_message(order_data_t* order, uint16_t port)
{
    char msg[LENGHT_MESSAGE];
    memcpy(msg, order, LENGHT_MESSAGE); //convert the order struct into sendable char*
    udp_broadcast(port, msg, LENGHT_MESSAGE+2); //+2 is not needed, be careful of the termination though
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
