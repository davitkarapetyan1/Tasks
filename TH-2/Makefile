# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread -fPIC
LDFLAGS = -shared

# Targets
all: demo-application

# Build the shared library (parallel_scheduler.so)
parallel_scheduler.so: parallel_scheduler.cpp parallel_scheduler.h
	$(CXX) $(CXXFLAGS) -o $@ -c parallel_scheduler.cpp

# Build the demo application
demo-application: demo-application.cpp parallel_scheduler.so
	$(CXX) $(CXXFLAGS) -o $@ demo-application.cpp -L. -lparallel_scheduler

# Clean up the generated files
clean:
	rm -f parallel_scheduler.o parallel_scheduler.so demo-application


