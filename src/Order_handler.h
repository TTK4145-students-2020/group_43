#ifndef ORDER_HANDLER_H
#define ORDER_HANDLER_H

#include <stdint.h>
#include <stdio.h>
#include "globals.h"


void order_update_queue(order_data_t received_order); //
void order_update_elevator(elevator_data_t received_elevator);
void order_initOrderList();

#endif //ORDER_HANDLER_H

