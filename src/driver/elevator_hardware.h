#pragma once

#include "../globals.h"

void elevator_hardware_init();

void elevator_hardware_set_motor_direction(Dirn dirn);
void elevator_hardware_set_button_lamp(Button button, int floor, int value);
void elevator_hardware_set_floor_indicator(int floor);
void elevator_hardware_set_door_open_lamp(int value);
void elevator_hardware_set_stop_lamp(int value);

int elevator_hardware_get_button_signal(Button button, int floor);
int elevator_hardware_get_floor_sensor_signal(void);
int elevator_hardware_get_stop_signal(void);
int elevator_hardware_get_obstruction_signal(void);



