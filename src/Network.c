#include "Network.h"

void msg network_receive_message(const char* ip, char* data, int datalength);

void network_init()
{
    printf("Init the network ...\n");
    //listening to the other elevators
    udp_startReceiving(CLIENT_PORT, network_receive_message);
}

void network_receive_message(const char* ip, char* data, int datalength)
{
    printf("Received UDP message from %s\n", ip);
    order_data_t received_order;
    memcpy(received_order, data, LENGHT_MESSAGE); // convert the message in a readable order
    /*printf("if this string is the orderList struct, then we receive these data\n%d\t%d\t%d\t%d\t%f\n",
        receivedMessage.data.a, receivedMessage.data.b, receivedMessage.data.c, receivedMessage.data.d, receivedMessage.data.f);
        */
    Order_update_queue(&received_order);
}

void network_boadcast_message(order_data_t* order)
{
    char msg[LENGHT_MESSAGE];
    memcpy(msg, order, LENGHT_MESSAGE); //convert the order struct into sendable char*
    udp_broadcast(SERVER_PORT, msg, LENGHT_MESSAGE);
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