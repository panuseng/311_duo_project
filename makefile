# Compiler and flags
CC = cc
CFLAGS = `pkg-config --cflags raylib`
LDFLAGS = `pkg-config --libs raylib` -lpthread

# Targets
TARGETS = client server

# Source files
CLIENT_SOURCES = client.c
SERVER_SOURCES = Server.c # Capital S to match your filename

# Object files
CLIENT_OBJECTS = $(CLIENT_SOURCES:.c=.o)
SERVER_OBJECTS = $(SERVER_SOURCES:.c=.o)

# Default rule
all: $(TARGETS)

# Rule to compile the client executable
client: $(CLIENT_OBJECTS)
	$(CC) $(CLIENT_OBJECTS) $(LDFLAGS) -o $@

# Rule to compile the server executable
server: $(SERVER_OBJECTS)
	$(CC) $(SERVER_OBJECTS) $(LDFLAGS) -o $@

# Rule to compile each object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the project (remove object files and executables)
clean:
	rm -f $(CLIENT_OBJECTS) $(SERVER_OBJECTS) $(TARGETS)
