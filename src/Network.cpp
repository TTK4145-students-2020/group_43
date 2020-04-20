#include "Network.h"
#include "elevator.h"


void network_broadcastMessage(message_t* order);
void network_receive_message(const char* ip, char* data, int datalength);
void network_forwardMessage(char* msg);
void network_printRawMessage(char* msg, uint16_t size);


message_t received_msg;
uint8_t probaRandomError;
bool elevatorRecovered = false;


void network_init(uint8_t probaErr)
{	
    printf("Init the network id %u...\n",ID_ELEVATOR);
	udp_startReceiving(COMM_PORT, network_receive_message);
	
	/* initialize random seed for simulating network error: */
	srand (time(NULL));
	probaRandomError = probaErr;
	
	for (uint8_t i = 0; i < SIZE_BUFFER_MESSAGES ; i++)
	{
		network_freeBufferReceivedMessage(i);
		receiveMessageTimer[i] = new threadTimer(TIMEOUT_RECEIVE_MESSAGE);
	}
	printf("Init the network ... DONE\n\n");
}

void network_broadcast(order_data_t* order)
{
	message_t msg;
	msg.id = ID_ORDER_MESSAGE;
	msg.data.order = *order;
	network_broadcastMessage(&msg);
}

void network_broadcast(elevator_data_t* elData)
{
	#if DEBUG == true
	printf("SENDING:\n");
	elevator_print(*elData);
	printf("\n\n\n");
	#endif
	message_t msg;
	msg.id = ID_ELEVATOR_MESSAGE;
	msg.data.elevator = *elData;
	network_broadcastMessage(&msg);
}

void network_broadcastMessage(message_t* data)
{
	char msg[LENGHT_MESSAGE];
    memcpy(msg, data, LENGHT_MESSAGE); //convert the order struct into sendable char*
	for(uint8_t i = 0;i<NUMBER_MESSAGES;i++)
	{
		if(rand()%100 < probaRandomError) 
		{
			uint8_t flipByte = rand()%15;
			msg[flipByte] = (msg[flipByte] + 10) % 255; //create an error in the message
			printf("Adding error in message %u\n",i);
		}
		udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); //+2 is not needed, be careful of the termination though
		usleep(1000); //wait 1ms between all the messages
	}
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
	{
		if(elevatorRecovered)
			return true;
	}
	return false;
}

void network_receive_message(const char* ip, char* data, int datalength)
{
	//TODO: check that we don't receive our own message checking the IP Address.
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
	memcpy(&received_msg, msg, LENGHT_MESSAGE);
	switch (received_msg.id)
	{
		case ID_ORDER_MESSAGE:
		    #if DEBUG == true
			printf("received order for floor %d\n",(int) received_msg.data.order.floor);
			#endif
			if(requestHandler_toTakeAssignedRequest(received_msg.data.order))
			{
				fsm_onRequestButtonPress(received_msg.data.order.floor,received_msg.data.order.button);
			}
			break;
		case ID_ELEVATOR_MESSAGE:
		    #if DEBUG == true
			printf("received elevator state with floor %d\n",(int) received_msg.data.elevator.floor);
			#endif
			if(received_msg.data.elevator.id == ID_ELEVATOR && fsm_getElevator()->floor ==-1) //elevalor uninitialized need recovery
			{//don't go there :()
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
			//printf("elevator id %u is asking for recovery",received_msg.data.IdToRecover);
			elevator_data_t* otherElevators = requestHandler_getOtherElevators();
			for(uint8_t i = 0; i< NUMBER_ELEVATOR ; i++)
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

void network_printRawMessage(char* msg, uint16_t size)
{
	for (uint16_t i = 0;i<size;i++)
		printf("%d ",(int)msg[i]);
}