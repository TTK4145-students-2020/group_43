#include "Network.h"

uint8_t network_checkMessage(uint8_t pos);
void network_receive_message();
void network_translateMessageToOrder(char* msg,order_data_t* order);
void network_printRawMessage(char* msg, uint16_t size);
void network_freeBufferReceivedMessage(uint8_t position);
void network_checkAndRecoverTimesOut();

order_data_t received_order;
char receivedMessage[NUMBER_ELEVATOR][NUMBER_MESSAGES][LENGHT_MESSAGE];
uint8_t numberOfMessagesReceived[NUMBER_ELEVATOR];
order_timer* receiveMessageTimer[NUMBER_ELEVATOR];

//we want to be able to store all (NUMBER_MESSAGES) the messages (LENGHT_MESSAGE) sent by other elevators (NUMBER_ELEVATOR-1  +1). +1 for more freedom if something goes wrong



void network_receive_message(const char* ip, char* data, int datalength)
{
	//TODO: check that we do'nt receive our own message checking the IP Address.
	if (datalength !=LENGHT_MESSAGE)
	{
		printf("Unknown message\n");
		return;
	}
    printf("Received UDP message from %s:\n", ip);
	//network_printRawMessage(data,LENGHT_MESSAGE);
	
	uint8_t position;
	uint8_t positionFound = 0;
	//first char is the ID
	uint8_t id_message = data[POS_ID_MESSAGE];
	//check if we already received this message ID
	for(position = 0; position<NUMBER_ELEVATOR;position++)
	{
		if(receivedMessage[position][0][POS_ID_MESSAGE] == id_message) 
		{//exit the for while saving the position
			positionFound = 1;
			break; //exits the for to keep the postion in memory
		}
	}
	if (positionFound==0)
	{
		printf("//it is the first message with this ID that we receive\n");
		//look for a free place to store the comming messages
		network_checkAndRecoverTimesOut();
		for(position = 0; position<NUMBER_ELEVATOR;position++)
		{
			
			if(receivedMessage[position][0][POS_ID_MESSAGE] == 0)
			{//this place is free
				positionFound = 1;
				//start timer for receiving messages (we may not receive three messages, or had a bad ID. We have to free up the place after the timer is gone.
				memcpy(&received_order, receivedMessage[position][0], LENGHT_MESSAGE);
				receiveMessageTimer[position] = new order_timer(&received_order, TIMEOUT_RECEIVE_MESSAGE);   //Start timer
				break;
			}
		}
	}
	if (positionFound ==0)
		printf("\n/!\\ ERROR, we should have found a place to store the message at this point !!\n\n");
		//TODO: add a forward recovery
		
	printf("we gonna store this message in position %d ; %u to analyse it\n",position,numberOfMessagesReceived[position]);
	//now position is the place where we should store the data
	//add the message received to the group of the same messages	
	memcpy(receivedMessage[position][numberOfMessagesReceived[position]], data, LENGHT_MESSAGE);
	numberOfMessagesReceived[position]++;
	if (numberOfMessagesReceived[position] == NUMBER_MESSAGES)
	{
		printf("let us check if messages are identical\n");
		uint8_t correctMessage = network_checkMessage(position);
		if (correctMessage == ERROR_INCONSITANT_MESSAGE)
		{
			printf("\n /!\\We don't take into account this message as we received it noisy\n");
			return; //we don't take the message into account because we received it wrong
		}
		network_translateMessageToOrder(receivedMessage[position][correctMessage],&received_order);
		network_freeBufferReceivedMessage(position);
		//delete/stop timer
		order_update_queue(received_order); //no pointer because we want order_handler to copy the order.
	}    
}

void network_init()
{	
    printf("Init the network ...\n");
	udp_startReceiving(COMM_PORT, network_receive_message);
	memset(receivedMessage,0,sizeof(receivedMessage));
	memset(numberOfMessagesReceived,0,sizeof(numberOfMessagesReceived));
	sleep(2);
	printf("Init the network ... DONE\n");
}

	
void network_broadcastMessage(order_data_t* order)
{
    char msg[LENGHT_MESSAGE];
    memcpy(msg, order, LENGHT_MESSAGE); //convert the order struct into sendable char*
	for(uint8_t i = 0;i<NUMBER_MESSAGES;i++)
		udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); //+2 is not needed, be careful of the termination though
}

void network_askRecovery()
{
	char msg[1] = {(int8_t) ID_ASK_RECOVER}; //-1
	udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); //send message to the others to send their data over the network
	
}

void network_translateMessageToOrder(char* msg,order_data_t* order)
{
	memcpy(order, msg , LENGHT_MESSAGE);
}

uint8_t network_checkMessage(uint8_t pos)
{//here we considere that 2 identical messages is the good message.
//2 should be ceil(NUMBER_MESSAGES/2)
	for(uint8_t version_i = 0;version_i<NUMBER_MESSAGES-1;version_i++)
	{
		for(uint8_t version_j = version_i+1;version_j<NUMBER_MESSAGES;version_j++)
		{
			if (strcmp(receivedMessage[pos][version_i],receivedMessage[pos][version_j]) ==0)
				return version_i; //we found two indentical messages
		}
	}
	return ERROR_INCONSITANT_MESSAGE;
}
	
void network_printRawMessage(char* msg, uint16_t size)
{
	for (uint16_t i = 0;i<size;i++)
		printf("%d ",(int)msg[i]);
}

void network_freeBufferReceivedMessage(uint8_t position)
{
	numberOfMessagesReceived[position] = 0;
	for (uint8_t msg=0;msg<NUMBER_MESSAGES;msg++)
		memset(receivedMessage[position][msg],0,LENGHT_MESSAGE);
	//free the timer?
}

void network_checkAndRecoverTimesOut()
{
	for(uint8_t pos = 0; pos< NUMBER_ELEVATOR; pos++)
	{
		if(receiveMessageTimer[pos]->is_timed_out())
		{
			printf("Time out for message at position %d. Checking if we ca extract data\n",pos);
			uint8_t num_msg = network_checkMessage(pos);
			if (num_msg != ERROR_INCONSITANT_MESSAGE)
			{
				printf("we could save the message\n");
				memcpy(&received_order,receivedMessage[pos][num_msg],LENGHT_MESSAGE);
				order_update_queue(received_order);
			}
			receiveMessageTimer[pos]->stop_timer();
		}
				
	}
}