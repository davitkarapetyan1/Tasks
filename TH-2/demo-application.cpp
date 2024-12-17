// demo-application.cpp

#include <iostream>
#include "parallel_scheduler.h"

void task(int id) {
    std::cout << "Task " << id << " is being executed by thread " << std::this_thread::get_id() << "\n";
}

int main() {
    // Create a parallel scheduler with a capacity of 3 threads
    parallel_scheduler scheduler(3);

    // Enqueue tasks (more than the pool capacity)
    for (int i = 0; i < 10; ++i) {
        scheduler.run([i]() { task(i); });
    }

    // Give enough time for tasks to complete before exiting
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}

