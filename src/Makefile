CC = clang++

CFLAGS = -Wall -g -std=c99 
LDFLAGS = -lpthread -g
SRC = main.cpp threadTimer.cpp threadTimerImplementation.cpp Network.cpp sverresnetwork.cpp requestHandler.cpp costFunc.cpp fsm.cpp elevator.c elevator_io_device.cpp driver/elevator_hardware.c requests.cpp timer.cpp
SRC_network = test_network.cpp threadTimer.cpp threadTimerImplementation.cpp Network.cpp sverresnetwork.cpp Order_handler.cpp 
TARGET =  elevator_project

#SRC = $(patsubst ./%, %, $(shell find . -name '*.c'  ! -name '*test*'))
#CXXSRC = $(patsubst ./%, %, $(shell find . -name '*.cpp'  ! -name '*test*'))



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

# does everything to tst the network
network: $(SRC_network)
	$(CC) $(SRC_network) -o $@ $(LDFLAGS)
	
# does everything to tst the requestHandler
rH_DEP := test_requestHandler.c requestHandler.c costFunc.c requests.c elevator.c elevator_io_device.c driver/elevator_hardware.c
requestHandler: test_requestHandler.c
	$(CC) $(rH_DEP) -o test_requestHandler -lpthread -Wall

	
.PHONY: all rebuild clean
