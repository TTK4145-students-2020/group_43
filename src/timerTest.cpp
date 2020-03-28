
#include <stdio.h>
#include <unistd.h>
#include "globals.hpp"
#include "orderTimer.hpp"

int main(){
    char c = 0;
    double duration = 3;
    
    while(1){
        printf("\ninitate timer? press y then enter:");
        while (c != 'y')
        {
            scanf("%c", &c);
        }
        order myOrder = { .data = 0};
        orderTimer myTimer(&myOrder, duration);               //Starts timer
        printf("\ntimeout: %i", myTimer.isTimedOut());    //Check flag
        int i = 0;
        while (!myTimer.isTimedOut())
        {
            //printf("timeout: %i", myTimer.isTimedOut());
            if (i==2)
            {
                printf("\nResetting timer");
                myTimer.resetTimer();
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