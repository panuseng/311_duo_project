# Compiler and flags
CC = cc
CFLAGS = `pkg-config --cflags raylib`
LDFLAGS = `pkg-config --libs raylib` -lpthread

# Targets
TARGET = client

# Source files
SOURCES = client.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default rule
all: $(TARGET)

# Rule to compile the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# Rule to compile each object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the project (remove object files and executable)
clean:
	rm -f $(OBJECTS) $(TARGET)
