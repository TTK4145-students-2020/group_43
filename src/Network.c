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
    network_message_t receivedMessage;
    memcpy(receivedMessage.message, data, LENGHT_MESSAGE);
    printf("if this string is the orderList struct, then we receive these data\n%d\t%d\t%d\t%d\t%f\n",
        receivedMessage.data.a, receivedMessage.data.b, receivedMessage.data.c, receivedMessage.data.d, receivedMessage.data.f);
}

void network_boadcast_message(network_message_t* msg)
{
    udp_broadcast(SERVER_PORT, msg.message, LENGHT_MESSAGE);
}

int main() {

    while (1)
    {
        printf("try to send a message\n");
        udp_send(COMPUTER_9_IP, 4320, orderList.message, LENGHT_MESSAGE); //port, data, length=64 in this case
        sleep(2);
        //	udp_send(USING_IP,4321, "send nudes" ,60); //port, data, length
        sleep(2);
    }
    return 0;
}