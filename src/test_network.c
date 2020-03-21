#include "Network.h"
#include <stdio.h>
#include <unistd.h>

void order_update_queue(order_data_t* received_order)
{
    printf("Order ID number %u received\n", received_order->id);
}

int main()
{
    order_data_t order;
    
    order.id = 0;
    order.floor = 0;
    order.direction = 1;
    order.owner = -1;
    order.destination_floor = 2;

    network_init();
    uint8_t i = 0;
    while(1)
    {
        order.id = i++;
        printf("send message ...\n");
        network_broadcast_message(&order);
        sleep(2);
    }
    return 0;
}
