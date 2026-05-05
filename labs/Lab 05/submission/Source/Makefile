# Compiler settings
CXX = g++
CC = gcc

# Include directories: 
# -I./include allows <GL/freeglut.h>
# -I./include/SOIL allows "SOIL.h" to be found by the C files
INCLUDES = -I./include -I./include/SOIL

# Compiler flags
CXXFLAGS = $(INCLUDES) -O2 -Wall
CFLAGS = $(INCLUDES) -O2 -Wall

# Linker flags (Library directory and specific libraries)
# Note: Order is important for GCC!
LDFLAGS = -L./lib -lfreeglut -lopengl32 -lglu32

# Source files (matching your exact filenames)
CPP_SRCS = src/main.cpp
C_SRCS = src/soil.c src/image_helper.c src/image_dxt.c src/stb_image_aug.c

# Output executable name
TARGET = main.exe

# Default target: compile the program
all: $(TARGET)

# Rule to compile everything into the executable
$(TARGET): $(CPP_SRCS) $(C_SRCS)
	$(CXX) $(CXXFLAGS) $(CPP_SRCS) $(C_SRCS) -o $(TARGET) $(LDFLAGS)

# Target to easily run the program after compiling
run: $(TARGET)
	.\$(TARGET)

# Rule to clean up the compiled file (Windows command)
clean:
	del $(TARGET)