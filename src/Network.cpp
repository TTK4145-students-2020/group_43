#include "Network.h"
#include "requestHandler.h"
#include "fsm.h"
#include "globals.hpp"

void network_broadcastMessage(message_t* order);
void network_receive_message(const char* ip, char* data, int datalength);
void network_forwardMessage(char* msg);
void network_freeBufferReceivedMessage(uint8_t position);
void network_checkAndRecoverTimesOut();
uint8_t getOldestMessage();
void network_printRawMessage(char* msg, uint16_t size);


message_t received_msg;
char receivedMessage[SIZE_BUFFER_MESSAGES][NUMBER_MESSAGES][LENGHT_MESSAGE];
//we want to be able to store all (NUMBER_MESSAGES) the messages (LENGHT_MESSAGE) sent by other elevators including possible errors(SIZE_BUFFER_MESSAGES)
uint8_t numberOfMessagesReceived[SIZE_BUFFER_MESSAGES];
threadTimer* receiveMessageTimer[SIZE_BUFFER_MESSAGES];
uint8_t probaRandomError;



void network_init(uint8_t probaErr)
{	
    printf("Init the network ...\n");
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
			printf("error in message %u\n",i);
		}
		udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); //+2 is not needed, be careful of the termination though
		usleep(1000); //wait 1ms between all the messages
	}
}

void network_askRecovery()
{ //send message to the others to send their data over the network
	message_t recoveryMsg;
	recoveryMsg.id = ID_ASK_RECOVER;
	recoveryMsg.data.IdToRecover = ID_ELEVATOR;
	network_broadcastMessage(&recoveryMsg);
}

void network_receive_message(const char* ip, char* data, int datalength)
{
	//TODO: check that we don't receive our own message checking the IP Address.
	if (datalength !=LENGHT_MESSAGE)
	{
		printf("Unknown message\n");
		return;
	}
    printf("Received UDP message from %s \t ID %u\n", ip,data[0]);
    //network_printRawMessage(data,LENGHT_MESSAGE);
	
	uint8_t position;
	uint8_t positionFound = 0;
	
	//check if we already received this message
	for(position = 0; position<SIZE_BUFFER_MESSAGES;position++)
	{
		if(strcmp(receivedMessage[position][0],data) == 0) 
		{
			positionFound = 1;
			break;
		}
	}
	if (positionFound==0)
	{
		printf("//it is the first message with this ID that we receive\n");
		//look for a free place to store the comming messages
		network_checkAndRecoverTimesOut();
		for(position = 0; position<SIZE_BUFFER_MESSAGES;position++)
		{
			
			if(numberOfMessagesReceived[position] == 0)
			{//this place is free
				positionFound = 1;
				receiveMessageTimer[position]->start();//we may not receive three messages, or had a bad ID. We have to free up the place after the timer is gone.
				break;
			}
		}
	}
	if (positionFound ==0)
	{
		printf("\n/!\\ ERROR, we should have found a place to store the message at this point !!\n\n");
		printf("We delete the oldest message\n");
		position = getOldestMessage();
		network_freeBufferReceivedMessage(position);
	}
	printf("we store this message in position %d ; %u to analyse it\n",position,numberOfMessagesReceived[position]);
	//now position is the place where we should store the data
	
	//add the message received to the group of the same messages	
	memcpy(receivedMessage[position][numberOfMessagesReceived[position]], data, LENGHT_MESSAGE);
	numberOfMessagesReceived[position]++;
	if (numberOfMessagesReceived[position] == NUMBER_MESSAGES)
	{//We received NUMBER_MESSAGES times the same message, no error!
		network_forwardMessage(receivedMessage[position][0]);
		network_freeBufferReceivedMessage(position);
		receiveMessageTimer[position]->stop();
	}    
}

void network_forwardMessage(char* msg)
{
	memcpy(&received_msg, msg, LENGHT_MESSAGE);
	switch (received_msg.id)
	{
		case ID_ORDER_MESSAGE:
			printf("received order for floor %d\n",(int) received_msg.data.order.floor);
			if(requestHandler_toTakeAssignedRequest(received_msg.data.order)){
				fsm_onRequestButtonPress(received_msg.data.order.floor,received_msg.data.order.button);
			}
			//order_update_queue(received_msg.data.order); //no pointer because we want order_handler to copy the order.
			break;
		case ID_ELEVATOR_MESSAGE:
			printf("received elevator state with floor %d\n",(int) received_msg.data.elevator.floor);
			if(received_msg.data.elevator.id == ID_ELEVATOR) {
				printf("recieved backup\n");
				//can return the backup as elevator message, then the elevatorid would be our id
				//fsm_initFromBackup(received_msg.data.elevator);
			}
			else {
				requestHandler_updateOtherElevators(received_msg.data.elevator);
			}
			//fsm_updateOtherElevators(received_msg.data.elevator); //no pointer because we want order_handler to copy the order.
			break;
		case ID_ASK_RECOVER:
			printf("\nask for recovery\n\n");
			//network_broadcast(requestHandler_getElevatorBackup(received_msg.id));
			//network_broadcastMessage(requestHandler_getElevator(receivedMessage.recoveryId);				
			break;
		default:
			printf("Unknown message id %u, we may have lost some data\n",received_msg.id);
	}
}

void network_freeBufferReceivedMessage(uint8_t position)
{
	numberOfMessagesReceived[position] = 0;
	for (uint8_t msg=0;msg<NUMBER_MESSAGES;msg++)
		memset(receivedMessage[position][msg],0xFF,LENGHT_MESSAGE); //set default to msg to only 1 because only 0 is a valide message
}

void network_checkAndRecoverTimesOut()
{
	for(uint8_t pos = 0; pos< SIZE_BUFFER_MESSAGES; pos++)
	{
		if (receiveMessageTimer[pos]->isTimedOut())
		{
			printf("Time out for message at position %d. Checking if we ca extract data\n",pos);
			if (numberOfMessagesReceived[pos]>= NUMBER_MESSAGES/2+1) //we need more than half the messages to correct
			{
				printf("we could save the message\n");
				network_forwardMessage(receivedMessage[pos][0]);
				network_freeBufferReceivedMessage(pos);
			}
			receiveMessageTimer[pos]->stop();
		}
	}
}

uint8_t getOldestMessage()
{
	double longestTime = 0;
	uint8_t indexMsgToDelete = SIZE_BUFFER_MESSAGES-1;
	for (uint8_t i = 0 ; i < SIZE_BUFFER_MESSAGES ; i++)
	{
		if (receiveMessageTimer[i]->getTime() > longestTime)
		{
			indexMsgToDelete = i;
			longestTime = receiveMessageTimer[i]->getTime();
		}
	}
	return indexMsgToDelete;
}

void network_printRawMessage(char* msg, uint16_t size)
{
	for (uint16_t i = 0;i<size;i++)
		printf("%d ",(int)msg[i]);
}