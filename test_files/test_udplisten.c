#include <stdio.h>
#include <unistd.h>
#include "../src/sverresnetwork.h"

void udpmessageReceived(const char * ip, char * data, int datalength){

  // Assuming an ascii string here - a binary blob (including '0's) will
  // be ugly/truncated.
  printf("Received UDP message from %s: '%s'\n",ip,data);
  
}

int main(){

  udp_startReceiving(4321,udpmessageReceived);
	while(1) 
	{
		udp_broadcast(4321,"Hello all",10);
		sleep(1);
	}
  return 0;
}

