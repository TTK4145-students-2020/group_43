#include "costFunc.h"
#include "requestHandler.h"

#include "con_load.h"
#include "elevator.h"
#include "elevator_io_device.h"
#include "requests.h"
#include "timer.h"

#include <stdio.h>



int main()
{
    
    Elevator elevator;
    Elevator otherElevators[2];
    order_data_t ord1;
    order_data_t ord2;
    order_data_t ord3;

    elevator = elevator_uninitialized();
    con_load("elevator.con",
        con_val("doorOpenDuration_s", &elevator.config.doorOpenDuration_s, "%lf")
        con_enum("clearRequestVariant", &elevator.config.clearRequestVariant,
            con_match(CV_All)
            con_match(CV_InDirn)
        )
    )
    for (int i=0;i<2;i++) {
        otherElevators[i] = elevator_uninitialized();
        con_load("elevator.con",
            con_val("doorOpenDuration_s", &otherElevators[i].config.doorOpenDuration_s, "%lf")
            con_enum("clearRequestVariant", &otherElevators[i].config.clearRequestVariant,
                con_match(CV_All)
                con_match(CV_InDirn)
            )
    )
    }
    elevator.id = 0;
    otherElevators[0].id = 1;
    otherElevators[1].id = 2;


    elevator.floor = 0;
    elevator.requests[1][B_Cab] = 1;
    elevator.requests[2][B_Cab] = 1;
    elevator.behaviour = EB_DoorOpen;
    elevator_print(elevator);

    otherElevators[0].floor = 1;
    otherElevators[0].requests[3][B_HallDown] = 1;
    otherElevators[0].behaviour = EB_DoorOpen;
    elevator_print(otherElevators[0]);

    otherElevators[1].floor = 0;
    otherElevators[1].behaviour = EB_DoorOpen;
    elevator_print(otherElevators[1]);

    ord1 = requestHandler_assignNewRequest(elevator, otherElevators, 3, B_Cab);
    ord2 = requestHandler_assignNewRequest(elevator, otherElevators, 2, B_HallUp);
    ord3 = requestHandler_assignNewRequest(elevator, otherElevators, 2, B_HallDown);

    printf("Owner of ord1 is: %d\n" , ord1.owner);
    printf("Owner of ord2 is: %d\n" , ord2.owner);
    printf("Owner of ord3 is: %d\n" , ord3.owner);

    return 1;

}