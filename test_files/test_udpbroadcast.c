#include <stdio.h>
#include <unistd.h>
#include "../src/sverresnetwork.h"

void 
udpmessageReceived(const char * ip, int port, char * data, int datalength){

  printf("Received UDP message from %s:%d: '%s'\n",ip,port,data);
  
}

int 
main(){
	
	while (1) 
	{
		printf("sending hello\n");
		udp_broadcast(4321,"Hello all",10);
		sleep(2);
	}

  return 0;
}

