#include "Network.h"

void udpmessageReceived(const char* ip, char* data, int datalength)
{
    printf("Received UDP message from %s: '%s'\n", ip, data);
    network_message_t receivedMessage;
    memcpy(receivedMessage.message, data, LENGHT_MESSAGE);
    printf("if this string is the orderList struct, then we receive these data\n%d\t%d\t%d\t%d\t%f\n",
        receivedMessage.data.a, receivedMessage.data.b, receivedMessage.data.c, receivedMessage.data.d, receivedMessage.data.f);
}

int main() {
    printf("hello\n\n"); //beginning of the code
    //listening to the other elevators
    udp_startReceiving(4321, udpmessageReceived);

    orderList_t orderList;
    //set some data in the union = the list of order
    memcpy(orderList.data.msg, "hello from Santid", 20);
    orderList.data.a = -1;
    orderList.data.b = -300;
    orderList.data.c = 300;
    orderList.data.d = 100;
    orderList.data.f = 1.3;

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