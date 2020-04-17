#ifndef ELEVATOR_IO_TYPES_H
#define ELEVATOR_IO_TYPES_H

#include "globals.h"


typedef struct {
    int (*floorSensor)(void);
    int (*requestButton)(int, Button);
    int (*stopButton)(void);
    int (*obstruction)(void);
    
} ElevInputDevice;

typedef struct {
    void (*floorIndicator)(int);
    void (*requestButtonLight)(int, Button, int);
    void (*doorLight)(int);
    void (*stopButtonLight)(int);
    void (*motorDirection)(Dirn);
} ElevOutputDevice;


char* elevio_dirn_toString(Dirn d);
char* elevio_button_toString(Button b);
#endif //ELEVATOR_IO_TYPES_H