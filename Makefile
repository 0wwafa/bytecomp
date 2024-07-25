# Name of the executable
TARGET = bytecomp

# Compiler flags
CFLAGS = -Wall -g -O3

# Linker flags (include -lm for math functions)
LDFLAGS = -lm

# All object files
OBJS = bytecomp.o

# Installation directory (change this if needed)
PREFIX = /usr/local

# Default target (build the executable)
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Install target
install: $(TARGET)
	install -D $(TARGET) $(PREFIX)/bin/$(TARGET)

# Clean target (remove object files and executable)
clean:
	rm -f $(OBJS) $(TARGET)

# Uninstall target
uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
