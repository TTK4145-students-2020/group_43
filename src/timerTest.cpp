
#include <stdio.h>
#include <unistd.h>
#include "threadTimer.hpp"

int main(){
    double duration = 6;
    threadTimer myTimer(duration);
    printf("alive?  %d\n", myTimer.isAlive());
    myTimer.start();               //Starts timer
    printf("timeout: %i\n", myTimer.isTimedOut());  //fflush(stdout); //Check flag 
    int i = 0;
    while (!myTimer.isTimedOut())
    {
        
        if (!(i % 5))
            printf("timeout: %i ", myTimer.isTimedOut()); 
            printf("alive?  %d  ", myTimer.isAlive());  
            printf("timer:    %f\n", myTimer.getTime());
        if (i == 3*10)
            myTimer.start();
            //myTimer.stop();
        usleep(100000);
        i++;
    }
    printf("alive?  %d", myTimer.isAlive());
    printf("timeout: %i\n", myTimer.isTimedOut());
    printf("end\n");
    return 0;
}