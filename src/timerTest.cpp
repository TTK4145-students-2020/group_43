
#include <stdio.h>
#include <unistd.h>

#include "globals.hpp"
#include "order_timer.hpp"

int main(){
    char c = 0;
    double duration = 0.04;
    
    while(1){
        printf("\ninitate timer? press y then enter:");
        while (c != 'y')
        {
            scanf("%c", &c);
        }
        order_data_t myOrder = { .floor=0};

        order_timer myTimer(&myOrder, duration);            //Starts timer
        for(int i = 0 ; i< 1000 ; i++)
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