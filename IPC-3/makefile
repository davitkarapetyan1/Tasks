CC = g++
CFLAGS = -std=c++11 -Wall -pthread
LDFLAGS = -lrt

# Все цели по умолчанию
all: first second

# Правила для first
first: first.cpp shared_array.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Правила для second
second: second.cpp shared_array.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Правила для shared_array
shared_array.o: shared_array.cpp shared_array.h
	$(CC) $(CFLAGS) -c shared_array.cpp

# Очистка
clean:
	rm -f first second shared_array.o

