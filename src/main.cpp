#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "con_load.h"
#include "elevator_io_device.h"
#include "fsm.h"
#include "timer.h"

#include "requestHandler.h"
#include "Network.h"
#include "threadTimer.h"
#include "globals.h"

uint8_t ID_ELEVATOR = 1;

void handleDeadElevators();
void getPointerToAllElevatorPointers(elevator_data_t** allElev);

int main(int argc,char** argv){
    printf("Started!\n");
	
	if(argc ==1)
	{
		printf("\nPlease add arguments: ");
		printf("First one is the elevator ID\n");
	}
	if(argc>1)
	{
		ID_ELEVATOR = atoi(argv[1]);
	}
	
	network_init();
    
    int inputPollRate_ms = 25;
    con_load("elevator.con",
        con_val("inputPollRate_ms", &inputPollRate_ms, "%d")
    )
    
    ElevInputDevice input = elevio_getInputDevice();    
    
    elevator_data_t* p_elevator = fsm_getElevator(); 
	p_elevator->id = ID_ELEVATOR;
    elevator_data_t* p_otherElevators = requestHandler_getOtherElevators();

    network_busyAskRecovery(RECOVERY_TIMEOUT_DURATION); //this function wait until it has been recover or timeout
	
    if(input.floorSensor() == -1){
        fsm_onInitBetweenFloors();
    }
    
	while(1){
        { // Request button
            static int prev[N_FLOORS][N_BUTTONS];
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    int v = input.requestButton(f, static_cast<Button>(b));
                    if(v  &&  v != prev[f][b]){ //new request
                        request_data_t newRequest = requestHandler_assignNewRequest(p_elevator,f,(Button)b); 
                        printf("Buttonpress\tId of the responsible elevator is %d\n", newRequest.ownerId);
                        if(requestHandler_toTakeAssignedRequest(newRequest)) {
							fsm_onRequestButtonPress(f, (Button)b);
							//no broadcast here because it will be a broadcast of the new state of the elevator
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
            }
            prev = f;
        }
        
        { // Timer
            if(timer_timedOut()){
                fsm_onDoorTimeout();
                timer_stop();
            }
        }
    
        handleDeadElevators();
    
        usleep(inputPollRate_ms*1000);
    }
    return 1;
}

void handleDeadElevators(){
    elevator_data_t* pp_elevators[N_ELEVATORS];
	getPointerToAllElevatorPointers(pp_elevators); 
    
    static bool oldElevatorTimeOuts[N_ELEVATORS] = { };
    bool elevatorTimeOuts[N_ELEVATORS] = { };

    for (int i = 0; i < N_ELEVATORS; i++)
       { elevatorTimeOuts[i] = pp_elevators[i]->timer->isTimedOut();}
    for (int i = 0; i < N_ELEVATORS; i++)
    {
        if (elevatorTimeOuts[i] && (elevatorTimeOuts[i]) != oldElevatorTimeOuts[i])
        {
            printf("\nDEATH ANNOUNCEMENT: Elevator ID %d died.", pp_elevators[i]->id);
            for (int floor = 0; floor < N_FLOORS; floor++)
                for (int button = 0; button < N_BUTTONS; button++)
                    if (pp_elevators[i]->requests[floor][button])
                    {
                        if ((Button)button == B_Cab)
                            printf("CRITICAL WARNING!!: Someone is stuck in elevator ID = %d !! \n", pp_elevators[i]->id);
                        else 
                        {
                            request_data_t recoveredOrder = requestHandler_assignNewRequest(pp_elevators[0], floor, (Button)button);
                            if (requestHandler_toTakeAssignedRequest(recoveredOrder))
                                fsm_onRequestButtonPress(floor, (Button)button);
                            else
                                network_broadcast(&recoveredOrder);
                        }
                    }
            requestHandler_clearAllHallwayRequests(pp_elevators[i]);
        }
        oldElevatorTimeOuts[i] = elevatorTimeOuts[i];
    }
}

void getPointerToAllElevatorPointers(elevator_data_t** allElev)
{
    allElev[0] = fsm_getElevator();
    elevator_data_t* p_othersTemp = requestHandler_getOtherElevators();
    for (int i = 0; i < N_ELEVATORS-1; i++)
    {
        allElev[i+1] = p_othersTemp+i;
    }
}