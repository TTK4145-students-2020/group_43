
#include "fsm.h"
#include "Network.h"
#include "requestHandler.h"

static elevator_data_t      elevator;
static ElevOutputDevice     outputDevice;


static void __attribute__((constructor)) fsm_init(){
    elevator = elevator_uninitialized();
    
    con_load("elevator.con",
        con_val("doorOpenDuration_s", &elevator.config->doorOpenDuration_s, "%lf")
        con_enum("clearRequestVariant", &elevator.config->clearRequestVariant,
            con_match(CV_All)
            con_match(CV_InDirn)
        )
    )
/*
    for (int i = 0; i<NUMBER_ELEVATOR-1; i++) { //new
        otherElevators[i] = elevator_uninitialized();
        con_load("elevator.con",
			con_val("doorOpenDuration_s", &otherElevators[i].config->doorOpenDuration_s, "%lf")
			con_enum("clearRequestVariant", &otherElevators[i].config->clearRequestVariant,
				con_match(CV_All)
				con_match(CV_InDirn)
			)
		)
    }
*/
    outputDevice = elevio_getOutputDevice();
}


inline int max ( int a, int b ) { return a > b ? a : b; } 

void fsm_setAllLights(elevator_data_t otherElevators[]){
    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int btn = 0; btn < N_BUTTONS; btn++){
            int lightValue = 0;
            if (btn != B_Cab) {
                for (int i=0; i<NUMBER_ELEVATOR-1; i++) {
                    lightValue = max(lightValue, otherElevators[i].requests[floor][btn]);
                }
            }
            outputDevice.requestButtonLight(floor, Button(btn), max(lightValue, elevator.requests[floor][btn]));
        }
    }
}

void fsm_onInitBetweenFloors(void){
    if(elevator.floor == -1){ // no backup avalible
        outputDevice.motorDirection(D_Down);
        elevator.dirn = D_Down;
        elevator.behaviour = EB_Moving;
    }
    
}

void fsm_onRequestButtonPress(int btn_floor, Button btn_type){
    printf("\n\n%s(%d, %s)\n", __FUNCTION__, btn_floor, elevio_button_toString(btn_type));
    elevator_print(elevator);
    
    switch(elevator.behaviour){
        
    case EB_DoorOpen:
        if(elevator.floor == btn_floor){
            timer_start(elevator.config->doorOpenDuration_s);
        } else {
            elevator.requests[btn_floor][btn_type] = 1;
        }
        break;

    case EB_Moving:
        elevator.requests[btn_floor][btn_type] = 1;
        break;
        
    case EB_Idle:
        if(elevator.floor == btn_floor){
            outputDevice.doorLight(1);
            timer_start(elevator.config->doorOpenDuration_s);
            elevator.behaviour = EB_DoorOpen;
        } else {
            elevator.requests[btn_floor][btn_type] = 1;
            elevator.dirn = requests_chooseDirection(elevator);
            outputDevice.motorDirection(elevator.dirn);
            elevator.behaviour = EB_Moving;
        }
        break;
        
    }

    network_broadcast(&elevator);
	fsm_setAllLights(requestHandler_getOtherElevators());
    
    printf("\nNew state:\n");
    elevator_print(elevator);
}




void fsm_onFloorArrival(int newFloor){
    printf("\n\n%s(%d)\n", __FUNCTION__, newFloor);
    elevator_print(elevator);
    
    elevator.floor = newFloor;
    
    outputDevice.floorIndicator(elevator.floor);
    
    switch(elevator.behaviour){
    case EB_Moving:
        if(requests_shouldStop(elevator)){
            outputDevice.motorDirection(D_Stop);
            outputDevice.doorLight(1);
            printf("requests.cpp doesn't have access to clear orders if we have segfault here\n");
            elevator = requests_clearAtCurrentFloor(elevator);
            printf("managed to delete orders\n");
            timer_start(elevator.config->doorOpenDuration_s);
            //setAllLights(elevator);
            elevator.behaviour = EB_DoorOpen;
        }
        break;
    default:
        break;
    }
    
    network_broadcast(&elevator);
	fsm_setAllLights(requestHandler_getOtherElevators());

    printf("\nNew state:\n");
    elevator_print(elevator); 
}




void fsm_onDoorTimeout(void){
    printf("\n\n%s()\n", __FUNCTION__);
    elevator_print(elevator);
    
    switch(elevator.behaviour){
    case EB_DoorOpen:
        elevator.dirn = requests_chooseDirection(elevator);
        
        outputDevice.doorLight(0);
        outputDevice.motorDirection(elevator.dirn);
        
        if(elevator.dirn == D_Stop){
            elevator.behaviour = EB_Idle;
        } else {
            elevator.behaviour = EB_Moving;
        }
        
        break;
    default:
        break;
    }
    
    network_broadcast(&elevator);

    printf("\nNew state:\n");
    elevator_print(elevator);
}

elevator_data_t* fsm_getElevator() {
    return &elevator;
}

void fsm_initFromBackup(elevator_data_t elevBackup) {
	memcpy(&elevator, &elevBackup, sizeof(elevBackup));
    if(elevator.floor == -1){
        printf("bad backup recieved, ignoring\n");
        return;
    }
    outputDevice.motorDirection(elevator.dirn);
    outputDevice.floorIndicator(elevator.floor);
    switch(elevator.behaviour){
        
    case EB_DoorOpen:
        outputDevice.doorLight(1);
        timer_start(elevator.config->doorOpenDuration_s);
        break;

    case EB_Moving:
        break;
        
    case EB_Idle:
        break;
    }

    network_broadcast(&elevator);
	fsm_setAllLights(requestHandler_getOtherElevators());

    printf("initialized from backup, new state:\n");
    elevator_print(elevator);
}







