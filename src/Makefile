#CC = clang
CC := gcc
CXX := g++

CFLAGS = -Wall -g -std=c99 
LDFLAGS = -lpthread -g
SRC = $(patsubst ./%, %, $(shell find . -name '*.c'  ! -name '*test*'))
#CXXSRC = $(patsubst ./%, %, $(shell find . -name '*.cpp'  ! -name '*test*'))
TARGET = test_network test_requestHandler


# does everything to tst the network
network: test_network.c sverresnetwork.c Network.c
	gcc test_network.c Network.c sverresnetwork.c -o test_network -lpthread -Wall

# does everything to tst the requestHandler
rH_DEP := test_requestHandler.c requestHandler.c costFunc.c requests.c elevator.c elevator_io_device.c driver/elevator_hardware.c
requestHandler: test_requestHandler.c
	$(CC) $(rH_DEP) -o test_requestHandler -lpthread -Wall

all: $(TARGET)

# Define all object files.
OBJ = $(SRC:.c=.o)

# rule to link the program
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	$(CC) $(CFLAGS) -c  $< -o $@ 

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(OBJ)

rebuild:	clean all


.PHONY: all rebuild clean