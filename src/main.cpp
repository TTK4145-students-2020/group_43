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


int main(int argc,char** argv){
    printf("Started!\n");
	
	if(argc ==1)
		printf("\nPlease add arguments: \n");
		printf("First one is the elevator ID\n");
		printf("Second one is the probability of error while sending a message\n\n");
				
	ID_ELEVATOR = 1;
	uint8_t probaRandomError = 0;
	if(argc>1)
	{
		ID_ELEVATOR = atoi(argv[1]);
	}
	if(argc>2)
	{
		probaRandomError = atoi(argv[2]);
	}
	
    network_init(probaRandomError);
	
    int inputPollRate_ms = 25;
    con_load("elevator.con",
        con_val("inputPollRate_ms", &inputPollRate_ms, "%d")
    )
    
    ElevInputDevice input = elevio_getInputDevice();    
    
    network_askRecovery();
	sleep(3); //to give the constructor time to update elevator if there is a backup avalible
    if(input.floorSensor() == -1){
        fsm_onInitBetweenFloors();
    }
    elevator_data_t* p_elevator = fsm_getElevator(); 
    elevator_data_t* p_otherElevators = requestHandler_getOtherElevators();
    while(1){
        { // Request button
            static int prev[N_FLOORS][N_BUTTONS];
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    int v = input.requestButton(f, static_cast<Button>(b));
                    if(v  &&  v != prev[f][b]){
                        order_data_t newRequest = requestHandler_assignNewRequest(p_elevator,f,static_cast<Button>(b)); 
                        if(requestHandler_toTakeAssignedRequest(newRequest)) {
							fsm_onRequestButtonPress(f, static_cast<Button>(b));
							network_broadcast(p_elevator);
							fsm_setAllLights(p_otherElevators);
						}
						else {
                            printf("going to broadcast new request\n");
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
				network_broadcast(fsm_getElevator());
				//fsm_setAllLights();
            }
            prev = f;
        }
        
        
        { // Timer
            if(timer_timedOut()){
                fsm_onDoorTimeout();
                timer_stop();
				network_broadcast(p_elevator);
            }
        }
        // some checkout timerThread.timeout or is this handled?

        usleep(inputPollRate_ms*1000);
    }
}