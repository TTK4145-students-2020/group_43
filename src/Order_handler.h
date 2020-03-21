#ifndef ORDER_HANDLER_H
#define ORDER_HANDLER_H

//#include "Network.h"

typedef struct 
{
    uint8_t     id; //include the ortder order (what order did we received first)
    uint8_t     floor;
    bool        direction; //1 for up
    uint32_t    reception_time;
    int8_t      owner; //who is taking the order? -1 if nobody
    uint8_t     destination_floor;
}order_data_t;


void order_update_queue(order_data_t* received_order); //
#endif //ORDER_HANDLER_H

