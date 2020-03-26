
#include <stdio.h>
#include <unistd.h>

#include "globals.hpp"
#include "order_timer.hpp"

int main(){
    char c = 0;
    double duration = 4;
    
    while(1){
        printf("\ninitate timer? press y then enter:");
        while (c != 'y')
        {
            scanf("%c", &c);
        }
        order myOrder = { .data=0};

        order_timer myTimer(&myOrder, duration);            //Starts timer
        printf("\ntimeout: %i", myTimer.is_timed_out());    //Check flag
        for (int i = 0; i < 5; i++)
        {
            printf("\n main thread sleeping ..  \n");
            sleep(1);
            if (i==3)
            {
                //myTimer.stop_timer();     //Stops timer.
            }
        }
        printf("\ntimeout: %i", myTimer.is_timed_out());
        c = 'n';
    }
    return 0;
}