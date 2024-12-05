CC = g++
CFLAGS = -std=c++11 -O2 -pthread
TARGET = array-summary

all: $(TARGET)

$(TARGET): array-summary.cpp
	$(CC) $(CFLAGS) -o $(TARGET) array-summary.cpp

clean:
	rm -f $(TARGET)

