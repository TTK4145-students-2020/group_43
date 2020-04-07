#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "con_load.h"
#include "elevator_io_device.h"
#include "fsm.h"
#include "timer.h"

#include "requestHandler.h"
#include "Network.h"
#include "threadTimer.hpp"
#include "globals.hpp"


int main(void){
    printf("Started!\n");
    
    int inputPollRate_ms = 25;
    con_load("elevator.con",
        con_val("inputPollRate_ms", &inputPollRate_ms, "%d")
    )
    
    ElevInputDevice input = elevio_getInputDevice();    
    
	//init, add extra stuff
    if(input.floorSensor() == -1){
        fsm_onInitBetweenFloors();
    }
        
    while(1){
        { // Request button
            static int prev[N_FLOORS][N_BUTTONS];
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    int v = input.requestButton(f, b);
                    if(v  &&  v != prev[f][b]){
						//here was my point with doing this in fsm
						order_data_t newRequest = requestHandler_assignNewRequest(elevator,otherElevators,f,b); 
						if(requestHandler_toTakeAssignedRequest(elevator, newRequest)) {
							fsm_onRequestButtonPress(f, b);
							network_broadcast(&elevator_data_t ...);
							fsm_setAllLights();
						}
						else {
							network_broadcast(&newRequest);
						}   
                    }
                    prev[f][b] = v;
                }
            }
        }
        
        { // Floor sensor
            static int prev;
            int f = input.floorSensor();
            if(f != -1  &&  f != prev){
                fsm_onFloorArrival(f);
				network_broadcast(&elevator_data_t ...);
				fsm_setAllLights();
            }
            prev = f;
        }
        
        
        { // Timer
            if(timer_timedOut()){
                fsm_onDoorTimeout();
                timer_stop();
				network_broadcast(&elevator_data_t ...);
            }
        }
        // some checkout timerThread.timeout or is this handled?

        usleep(inputPollRate_ms*1000);
    }
}