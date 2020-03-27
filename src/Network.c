#include "Network.h"

order_data_t received_order;
char receivedMessage[NUMBER_ELEVATOR-1][NUMBER_MESSAGES][LENGHT_MESSAGE];
uint8_t numberOfMessagesReceived[NUMBER_ELEVATOR-1];
//we want to be able to store all (NUMBER_MESSAGES) the messages (LENGHT_MESSAGE) sent by other elevators (NUMBER_ELEVATOR-1)

uint8_t network_checkMessage(uint8_t pos);
void network_receive_message();
void network_translateMessageToOrder(char* msg,order_data_t* order);

void network_receive_message(const char* ip, char* data, int datalength)
{
	if (datalength !=LENGHT_MESSAGE)
	{
		printf("Unknown message\n");
		return;
	}
    printf("Received UDP message from %s\n", ip);
	
	uint8_t position = 0;
	//first char is the ID
	uint8_t id_message = data[0];
	//check if we already received this message ID
	for(position = 0; position<NUMBER_ELEVATOR-1;position++)
	{
		if(receivedMessage[position][0][0] == id_message) 
		{//exit the for while saving the position
			break; //TODO: should may be check that it really exits the for.
		}
	}
	//it is the first message with this ID that we receive:
	//look for a free place to store the comming messages
	for(position = 0; position<NUMBER_ELEVATOR-1;position++)
	{
		if(receivedMessage[position][0][0] == 0)
		{//this place is free
			break;
		}
	}
	//now position is the place where we should store the data
	//add the message received to the group of the same messages	
	memcpy(receivedMessage[position][numberOfMessagesReceived[position]], data, LENGHT_MESSAGE);
	numberOfMessagesReceived[position]++;
	if (numberOfMessagesReceived[position] == NUMBER_MESSAGES)
	{
		uint8_t correctMessage = network_checkMessage(position);
		if (correctMessage == ERROR_INCONSITANT_MESSAGE)
			return; //we don't take the message into account because we received it wrong
		network_translateMessageToOrder(receivedMessage[position][correctMessage],&received_order);
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
    udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); //+2 is not needed, be careful of the termination though
}

void network_askRecovery()
{
	char msg[1] = {(int8_t) ID_ASK_RECOVER}; //-1
	udp_broadcast(COMM_PORT, msg, LENGHT_MESSAGE); //send message to the others to send their data over the network
	
}

void network_translateMessageToOrder(char* msg,order_data_t* order)
{
	memcpy(msg, order, LENGHT_MESSAGE);
}

uint8_t network_checkMessage(uint8_t pos)
{//here we concidere that 2 identical messages is the good message.
	for(uint8_t version_i = 0;version_i<NUMBER_ELEVATOR-1;version_i++)
	{
		for(uint8_t version_j = 0;version_j<NUMBER_ELEVATOR-1;version_j++)
		{
			if(strcmp(receivedMessage[pos][version_i],receivedMessage[pos][version_j]))
				return version_i;
		}
	}
	return ERROR_INCONSITANT_MESSAGE;
}
	
	