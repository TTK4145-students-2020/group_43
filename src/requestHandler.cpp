#include "requestHandler.h"
#include "fsm.h"

float timeToServeRequest(elevator_data_t* e_old, Button b, int f);
void getPointerToAllElevatorPointers(elevator_data_t** allElev);

static elevator_data_t      otherElevators[N_ELEVATORS-1];

static void __attribute__((constructor)) requestHandler_init(){
    for (int i = 0; i<N_ELEVATORS-1; i++) {
        otherElevators[i] = elevator_uninitialized();
        con_load("elevator.con",
			con_val("doorOpenDuration_s", &otherElevators[i].config->doorOpenDuration_s, "%lf")
			con_enum("clearRequestVariant", &otherElevators[i].config->clearRequestVariant,
				con_match(CV_All)
				con_match(CV_InDirn)
			)
		)
    }
}

elevator_data_t* requestHandler_getOtherElevators(void) {
    return otherElevators;
}

elevator_data_t* requestHandler_getElevatorBackup(int elevId) {
    for(int i = 0; i<N_ELEVATORS-1; i++){
        if(otherElevators[i].id == elevId) {
            return &otherElevators[i];
        }
    }
    return NULL;
}

void requestHandler_updateOtherElevators(elevator_data_t newElevState) {
    if(newElevState.id == ID_ELEVATOR) return; //it was not an other elevator but ourselves
	//find out where elev with ip/id is stored locally
    int elevIndex = 0; 
    for(int i = 0; i<N_ELEVATORS-1; i++){
        if(otherElevators[i].id == -1){ //if this is the first time recieving an update form this elevator
            otherElevators[i].id = newElevState.id;
			printf("new elevator in the network, id %u\n",newElevState.id);
        }
        if(otherElevators[i].id == newElevState.id) {
            elevIndex = i;
            break;
        }
    }

    otherElevators[elevIndex].floor = newElevState.floor;
    otherElevators[elevIndex].dirn = newElevState.dirn;
    for(int f = 0; f<N_FLOORS; f++){
        for(int btn = 0; btn<N_BUTTONS; btn++){
            otherElevators[elevIndex].requests[f][btn] = newElevState.requests[f][btn];
        }
    }
    otherElevators[elevIndex].behaviour = newElevState.behaviour;

    if(otherElevators[elevIndex].behaviour == EB_Idle) {
        otherElevators[elevIndex].timer->stop();
    }
    else {
        otherElevators[elevIndex].timer->start();
    }
    fsm_setAllLights(otherElevators);
}

void requestHandler_clearAllHallwayRequests(elevator_data_t* e){
    for (int floor = 0; floor < N_FLOORS; floor++)
        for (int btn = 0; btn < N_BUTTONS-1; btn++)
            e->requests[floor][btn]=0;
}

int requestHandler_toTakeAssignedRequest(request_data_t assignedRequest) {
    if (assignedRequest.ownerId == ID_ELEVATOR) {
        return 1;
    }
    return 0;
}

// returns an assigned request, where the id of the chosen elevator is included
request_data_t requestHandler_assignNewRequest(elevator_data_t* elevator, int btn_floor, Button btn_type) {
    request_data_t newRequest;
    newRequest.floor = btn_floor;
    newRequest.button = btn_type;
    
    if (btn_type == B_Cab) {
        newRequest.ownerId = elevator->id;
        return newRequest;
    }
    
    float cost[N_ELEVATORS];
    for (int i = 0; i<N_ELEVATORS; i++) {
        cost[i] = INT_MAX;
    }

    cost[0] = timeToServeRequest(elevator, btn_type, btn_floor);
    int minCostIndex = 0;
    for (int i=1; i<N_ELEVATORS; i++) {
        if (!otherElevators[i-1].timer->isTimedOut() && otherElevators[i-1].floor > -1) {
            cost[i] = timeToServeRequest(&otherElevators[i-1], btn_type, btn_floor);
            if (cost[i] < cost[i-1]) {
                minCostIndex = i;
            }
        }
    }
    if (minCostIndex == 0) {
        newRequest.ownerId = elevator->id;
    }
    else {
        newRequest.ownerId = otherElevators[minCostIndex-1].id;
    }

    return newRequest;
}

float timeToServeRequest(elevator_data_t* e_old, Button b, int f)
{
    elevator_data_t e = *e_old;
	if(e.id ==-1)
		return std::numeric_limits<float>::max();
    const double DOOR_OPEN_TIME = e.config->doorOpenDuration_s;
    e.requests[f][b] = 1;

    float duration = 0;

    switch (e.behaviour)
    {
    case EB_Idle:
        e.dirn = requests_chooseDirection(e);
        if (e.dirn == D_Stop)
        {
            return duration;
        }
        break;
    case EB_Moving:
        duration += TRAVEL_TIME_BETWEEN_FLOOR  / 2;
        e.floor += e.dirn;
        break;
    case EB_DoorOpen:
        duration -= DOOR_OPEN_TIME / 2;
    }

    while (1)
    {
        if (requests_shouldStop(e))
        {
            for (int btn = 0; btn < N_BUTTONS; btn++)
            {
                if (e.requests[e.floor][btn])
                {
                    e.requests[e.floor][btn] = 0;
                    if (btn == b && e.floor == f)
                    {
                        //arrived at request
                        return duration;
                    }
                }
            }
            duration += DOOR_OPEN_TIME;
            e.dirn = requests_chooseDirection(e);
        }
        e.floor += e.dirn;
        duration += TRAVEL_TIME_BETWEEN_FLOOR;
    }
}

void requestHandler_handleDeadElevators(){
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