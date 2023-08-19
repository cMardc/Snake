# Makefile for Snake Game

# Compiler and flags
CC = gcc
CFLAGS = -lm

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source file
SOURCE = $(SRC_DIR)/main.c

# Executable file
EXECUTABLE = $(BUILD_DIR)/snake

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
