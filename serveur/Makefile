# Variables for the compiler
CC = gcc
CFLAGS = -Wall -g -pthread  # Add -pthread here for compilation
LDFLAGS = -lncurses -pthread  # Add -pthread here for linking

# Name of the executable
TARGET = bomberman_serv

# Source files
SOURCES = main_server.c game.c add_to_game.c launch_game.c player_ready.c manage_action.c bomb.c send_chat.c
OBJECTS = $(SOURCES:.c=.o)

# Default rule
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -f $(TARGET) $(OBJECTS)

# For 'make run'
run: $(TARGET)
	./$(TARGET)

# Check header dependencies
depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend;

include .depend
