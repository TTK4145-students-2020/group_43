#include "Network.h"

void network_broadcastMessage(message_t* request);
void network_receive_message(const char* ip, char* data, int datalength);
void network_forwardMessage(char* msg); //call the right function according to the jind of message we received

bool elevatorRecovered = false; //flag to know when the recover is done.


void network_init()
{	
    printf("Init the network id %u...\n",ID_ELEVATOR);
	udp_startReceiving(COMM_PORT, network_receive_message);
}

void network_broadcast(request_data_t* request)
{
	message_t msg;
	msg.id = ID_REQUEST_MESSAGE;
	msg.data.request = *request;
	network_broadcastMessage(&msg);
}

void network_broadcast(elevator_data_t* elData)
{
	message_t msg;
	msg.id = ID_ELEVATOR_MESSAGE;
	msg.data.elevator = *elData;
	network_broadcastMessage(&msg);
}

bool network_busyAskRecovery(float timeOut)
{ //send message to the others to send their data over the network
	printf("asking for recovery\n");
	elevatorRecovered = false;
	message_t recoveryMsg;
	recoveryMsg.id = ID_ASK_RECOVER;
	recoveryMsg.data.IdToRecover = ID_ELEVATOR;
	network_broadcastMessage(&recoveryMsg);
	
	threadTimer recoveryTimer(timeOut);
	recoveryTimer.start();
	while(!recoveryTimer.isTimedOut())
	{//busy wait
		if(elevatorRecovered)
			return true;
	}
	return false;
}

void network_broadcastMessage(message_t* data)
{
	char msg[LENGHT_MESSAGE];
    memcpy(msg, data, LENGHT_MESSAGE); //convert the request struct into sendable char*
	for(uint8_t i = 0;i<N_MESSAGES;i++)
	{
		udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); 
		usleep(800); //wait between all the messages
	}
}

void network_receive_message(const char* ip, char* data, int datalength)
{
	if (datalength !=LENGHT_MESSAGE)
	{
		printf("Unknown message\n");
		return;
	}
    #if DEBUG == true
	printf("Received UDP message from %s \t ID %u\n", ip,data[0]);
	#endif
	
	network_forwardMessage(data);
}

void network_forwardMessage(char* msg)
{
	message_t received_msg;
	memcpy(&received_msg, msg, LENGHT_MESSAGE);
	switch (received_msg.id)
	{
		case ID_REQUEST_MESSAGE:
		    #if DEBUG == true
			printf("received request for floor %d\n",(int) received_msg.data.request.floor);
			#endif
			if(requestHandler_toTakeAssignedRequest(received_msg.data.request))
			{
				fsm_onRequestButtonPress(received_msg.data.request.floor,received_msg.data.request.button);
			}
			break;
		case ID_ELEVATOR_MESSAGE:
		    #if DEBUG == true
			printf("received elevator state with floor %d\n",(int) received_msg.data.elevator.floor);
			#endif
			if(received_msg.data.elevator.id == ID_ELEVATOR && fsm_getElevator()->floor == UNINITIALIZED_FLOOR) 
			{//elevalor uninitialized need recovery
				printf("Recieved backup\n");
				if(fsm_initFromBackup(received_msg.data.elevator))
					elevatorRecovered = true;
			}
			else {
				requestHandler_updateOtherElevators(received_msg.data.elevator);
			}
			break;
		case ID_ASK_RECOVER:
		{
		    #if DEBUG == true
			printf("\nasked for recovery\n\n");
		    #endif
			elevator_data_t* otherElevators = requestHandler_getOtherElevators();
			for(uint8_t i = 0; i< N_ELEVATORS ; i++)
				if (otherElevators[i].id == received_msg.data.IdToRecover)
				{
					network_broadcast(&(otherElevators[i]));
					printf("Recovey possible, we found some elevator data\n");
				}
			network_broadcast(fsm_getElevator()); //to make our state known to initiated elevator
			break;
		}
		default:
			printf("Unknown message id %u, we may have lost some data\n",received_msg.id);
	}
}