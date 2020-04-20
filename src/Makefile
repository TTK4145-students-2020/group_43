CC = clang++

CFLAGS = -Wall -g -std=c99 
LDFLAGS = -lpthread -g
SRC = main.cpp threadTimer.cpp threadTimerImplementation.cpp Network.cpp sverresnetwork.cpp requestHandler.cpp fsm.cpp elevator.c elevator_io_device.cpp driver/elevator_hardware.c requests.cpp timer.cpp 
TARGET =  elevator_project


all: $(TARGET) 

# Define all object files.
OBJ = $(SRC:.cpp=.o)

# rule to link the program
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	$(CC) $(CFLAGS) -c  $< -o $@ 

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) *.o

rebuild:	clean all


.PHONY: all rebuild clean
