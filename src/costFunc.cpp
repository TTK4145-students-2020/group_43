#include "costFunc.h"

#include <stdio.h>
#include <limits>
#include "con_load.h"
#include "elevator_io_device.h"
#include "requests.h"

#define TRAVEL_TIME 2

float costFunc_timeToServeRequest(elevator_data_t* e_old, Button b, int f)
{
    elevator_data_t e = *e_old;
	if(e.id ==-1)
		return std::numeric_limits<float>::max();
    const double DOOR_OPEN_TIME = e.config->doorOpenDuration_s;
    e.requests[f][b] = 1;

    float duration = 0;

    switch (e.behaviour)
    {
    case EB_Idle:
        e.dirn = requests_chooseDirection(e);
        if (e.dirn == D_Stop)
        {
            return duration;
        }
        break;
    case EB_Moving:
        duration += TRAVEL_TIME / 2;
        e.floor += e.dirn;
        break;
    case EB_DoorOpen:
        duration -= DOOR_OPEN_TIME / 2;
    }

    while (1)
    {
        if (requests_shouldStop(e))
        {
            for (int btn = 0; btn < N_BUTTONS; btn++)
            {
                if (e.requests[e.floor][btn])
                {
                    e.requests[e.floor][btn] = 0;
                    if (btn == b && e.floor == f)
                    {
                        //arrived at request
                        return duration;
                    }
                }
            }
            duration += DOOR_OPEN_TIME;
            e.dirn = requests_chooseDirection(e);
        }
        e.floor += e.dirn;
        duration += TRAVEL_TIME;
    }
}
