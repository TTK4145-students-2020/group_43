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
elevator_data_t* getPointerToAllElevatorPointers();

int main(int argc,char** argv){
    printf("Started!\n");
	
	if(argc ==1)
		printf("\nPlease add arguments: \n");
		printf("First one is the elevator ID\n");
		printf("Second one is the probability of error while sending a message\n\n");
				
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
    
    network_busyAskRecovery(TIMEOUT_RECOVERY); //this function wait until it has been recover or timeout
	
    if(input.floorSensor() == -1){
        fsm_onInitBetweenFloors();
    }
    elevator_data_t* p_elevator = fsm_getElevator(); 
	p_elevator->id = ID_ELEVATOR;
    elevator_data_t* p_otherElevators = requestHandler_getOtherElevators();
	printf("ID of this elevator is %u",p_elevator->id);
    while(1){
        { // Request button
            static int prev[N_FLOORS][N_BUTTONS];
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    int v = input.requestButton(f, static_cast<Button>(b));
                    if(v  &&  v != prev[f][b]){
                        order_data_t newRequest = requestHandler_assignNewRequest(p_elevator,f,static_cast<Button>(b)); 
                        printf("Buttonpress\nowner of new order is %d\n", newRequest.owner);
                        if(requestHandler_toTakeAssignedRequest(newRequest)) {
							fsm_onRequestButtonPress(f, static_cast<Button>(b));
                            //network_broadcast(p_elevator);
				            //fsm_setAllLights(p_otherElevators);
						}
						else {
                            printf("going to broadcast new request\n");
							network_broadcast(&newRequest);
							//fsm_setAllLights(p_otherElevators); //TODO: is it neeeded?
						}   
                    }
					prev[f][b] = v;

                }
            }
        }
            handleDeadElevators();
        { // Floor sensor
            static int prev;
            int f = input.floorSensor();
            if(f != -1  &&  f != prev){
                fsm_onFloorArrival(f);
				//network_broadcast(p_elevator);
				//fsm_setAllLights(p_otherElevators);
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
        //fsm_setAllLights(p_otherElevators);
        // some checkout timerThread.timeout or is this handled?
        
        usleep(inputPollRate_ms*1000);
    }
    return 1;
}

void handleDeadElevators(){
    
    static elevator_data_t pp_elevators[NUMBER_ELEVATOR] = getPointerToAllElevatorPointers(); 
    static bool oldElevatorTimeOuts[NUMBER_ELEVATOR] = { };
    
    bool elevatorTimeOuts[NUMBER_ELEVATOR] = { };
    for (int i = 0; i < NUMBER_ELEVATOR; i++)
        elevatorTimeOuts[i] = pp_elevators[i]->timer->isTimedOut();
    for (int i = 0; i < NUMBER_ELEVATOR; i++)
    {
        if (elevatorTimeOuts[i] && (elevatorTimeOuts[i]) != oldElevatorTimeOuts[i])
        {
            printf("\nDEATH ANNOUNCEMENT: Elevator ID %d died.", pp_elevators[i].id);
            for (int floor = 0; floor < N_FLOORS; floor++)
                for (int button = 0; button < N_BUTTONS; button++)
                    if (pp_elevators[i].requests[floor][button])
                    {
                        if ((Button)button == B_Cab)
                            printf("CRITICAL WARNING!!: Someone is stuck in elevator ID = %d !! \n\n", pp_elevators[i].id);
                        else 
                        {
                            order_data_t recoveredOrder = requestHandler_assignNewRequest(p_elevator[i], floor, (Button)button);
                            if (requestHandler_toTakeAssignedRequest(recoveredOrder))
                                fsm_onRequestButtonPress(floor, (Button)button);
                            else
                                network_broadcast(&recoveredOrder);
                        }
                    }
            requestHandler_wipeHallwayRequests(pp_elevators[i]);
        }
        oldOtherElevatorTimedOut[i] = otherElevatorTimedOut[i];
    }
}

elevator_data_t** getPointerToAllElevatorPointers()
{
    elevator_data_t* pp_elevators[NUMBER_ELEVATOR];
    pp_elevator[0] = fsm_getElevator();
    elevator_data_t* pp_othersTemp = requestHandler_getOtherElevators();
    for (int i = 1; i < NUMBER_ELEVATOR; i++)
    {
        pp_elevators[i] = pp_othersTemp[i-1];
    }
    return pp_elevator;
}