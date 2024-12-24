# Makefile for signal-echo application

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Output executable
TARGET = signal-echo

# Default target
all: $(TARGET)

$(TARGET): signal-echo.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) signal-echo.cpp

# Clean up
clean:
	rm -f $(TARGET)

