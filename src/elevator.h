#pragma once

#include "elevator_io_types.h"
#include "threadTimer.hpp"
#include "globals.hpp"

typedef enum {
    // Assume everyone waiting for the elevator gets on the elevator, even if 
    // they will be traveling in the "wrong" direction for a while
    CV_All,
    
    // Assume that only those that want to travel in the current direction 
    // enter the elevator, and keep waiting outside otherwise
    CV_InDirn,
} ClearRequestVariant;

typedef struct {
	elevator_data_t			data; 
	threadTimer             timer;
    struct {
        ClearRequestVariant clearRequestVariant;
        double              doorOpenDuration_s;
    } config;    
} Elevator;


void elevator_print(Elevator es);

Elevator elevator_uninitialized(void);
