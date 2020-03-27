CC = clang 

CFLAGS = -Wall -g -std=c99 
LDFLAGS = -lpthread -g
SRC = test_network.c sverresnetwork.c Network.c
TARGET = test_network


# does everything to tst the network
network: test_network.c sverresnetwork.c Network.c
	gcc test_network.c Network.c sverresnetwork.c Order_handler.c -o test_network -lpthread -Wall

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
