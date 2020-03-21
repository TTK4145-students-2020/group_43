CC = clang++

CFLAGS = -Wall -g -std=c99 
LDFLAGS = -lpthread -g
SRC = timerTest.cpp order_timer.cpp order_timer_implementation.cpp

TARGET = timerTest


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
