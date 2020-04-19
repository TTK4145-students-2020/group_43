
#include "elevator.h"
#include "globals.h"
#include <stdio.h>

#include "timer.h"



char* eb_toString(ElevatorBehaviour_t eb){
    return (char*) (
        eb == EB_Idle       ? "EB_Idle"         :
        eb == EB_DoorOpen   ? "EB_DoorOpen"     :
        eb == EB_Moving     ? "EB_Moving"       :
                              "EB_UNDEFINED"   );
}



void elevator_print(elevator_data_t es){
    printf("  +--------------------+\n");
    printf(
        "  |floor = %-2d          |\n"
        "  |dirn  = %-12.12s|\n"
        "  |behav = %-12.12s|\n",
        es.floor,
        elevio_dirn_toString(es.dirn),
        eb_toString(es.behaviour)
    );
    printf("  +--------------------+\n");
    printf("  |  | up  | dn  | cab |\n");
    for(int f = N_FLOORS-1; f >= 0; f--){
        printf("  | %d", f);
        for(int btn = 0; btn < N_BUTTONS; btn++){
            if((f == N_FLOORS-1 && btn == B_HallUp)  || 
               (f == 0 && btn == B_HallDown) 
            ){
                printf("|     ");
            } else {
                printf(es.requests[f][btn] ? "|  #  " : "|  -  ");
            }
        }
        printf("|\n");
    }
    printf("  +--------------------+\n");
}

elevator_data_t elevator_uninitialized(void){
    printf("Initalizing timers..\n");
    return (elevator_data_t){
        .id = -1,
        .floor = -1,
        .dirn = D_Stop,
        .behaviour = EB_Idle,
        .timer = new threadTimer(ELEVATOR_TIMEOUT_DURATION),
        .config = new config_t {
            .clearRequestVariant = CV_All,
            .doorOpenDuration_s = 3.0,
        },
    };
}