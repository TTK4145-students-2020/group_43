//#include "Network.h"
#include "Order_handler.h"

void order_update_queue(order_data_t received_order)
{
    printf("Order floor number %u received\n\n\n", received_order.floor);
} 

void order_update_elevator(elevator_data_t received_elevator)
{
    printf("Elevator floor number %u received\n\n\n", received_elevator.floor);
} 