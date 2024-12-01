
CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = interactive-shell

OBJS = interactive-shell.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

interactive-shell.o: interactive-shell.cpp
	$(CXX) $(CXXFLAGS) -c interactive-shell.cpp

clean:
	rm -f $(OBJS) $(TARGET)

