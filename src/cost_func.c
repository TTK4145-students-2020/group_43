#include "cost_func.h"

#include <stdio.h>

#include "con_load.h"
#include "elevator_io_device.h"
#include "requests.h"

int timeToServeRequest(Elevator e_old, Button b, floor f){
    Elevator e = e_old;
    const double DOOR_OPEN_TIME = e.config.doorOpenDuration_s
    elevator.requests[f][b] = 1;

    int arrivedAtRequest = 0;
    void ifEqual(Button inner_b, int inner_f){
        if(inner_b == b && inner_f == f){
            arrivedAtRequest = 1;
        }
    }

    int duration = 0;
    
    switch(e.behaviour){
    case EB_Idle:
        e.dirn = requests_chooseDirection(e);
        if(e.dirn == D_Stop){
            return duration;
        }
        break;
    case EB_Moving:
        duration += TRAVEL_TIME/2;
        e.floor += e.dirn;
        break;
    case EB_DoorOpen:
        duration -= DOOR_OPEN_TIME/2;
    }


    while(true){
        if(requests_shouldStop(e)){
            e = requests_clearAtCurrentFloor(e, ifEqual);
            if(arrivedAtRequest){
                return duration;
            }
            duration += DOOR_OPEN_TIME;
            e.dirn = requests_chooseDirection(e);
        }
        e.floor += e.direction;
        duration += TRAVEL_TIME;
    }
}