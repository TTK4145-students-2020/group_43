
#include <stdio.h>
#include <unistd.h>
#include "globals.hpp"
#include "threadTimer.hpp"

int main(){
    char c = 0;
    double duration = 3;
    
    while(1){
        printf("\ninitate timer? press y then enter:");
        while (c != 'y')
        {
            scanf("%c", &c);
        }
        order_data_t myOrder = { .data = 0};
        threadTimer myTimer(duration);               //Starts timer
        printf("\ntimeout: %i", myTimer.isTimedOut());    //Check flag
        for (int i = 0; i < 5; i++)
        {
            //printf("timeout: %i", myTimer.isTimedOut());
            if (i==2)
            {
                
                myTimer.resetTimer(); printf("\nResetting timer");
                //myTimer.stopTimer(); printf("\nStopping timer");
            }
            printf("\nmain thread sleeping ..");
            sleep(1);
            i++;
        }
        
        printf("\ntimeout: %i", myTimer.isTimedOut());
        c = 'n';
    }
    return 0;
}