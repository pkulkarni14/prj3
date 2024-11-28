CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
LDFLAGS = -pthread

# Source files
SRCS = src/main.c src/server.c src/client_handler.c src/signal_handler.c src/logging.c src/server_context.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = chat_server

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compiling source files
%.o: %.c chat_server.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean