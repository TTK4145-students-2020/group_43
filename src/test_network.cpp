#include "Network.h"
#include <stdio.h>
#include <unistd.h>

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
        network_broadcast_message(&order);
        sleep(2);
    }
}
