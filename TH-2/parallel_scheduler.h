// parallel_scheduler.h

#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    // Enqueue a task (function) to the thread pool
    void run(std::function<void()> func);

private:
    // A function that each thread will execute
    void worker();

    size_t capacity_;  // Maximum number of threads in the pool
    std::vector<std::thread> workers_;  // Threads in the pool
    std::queue<std::function<void()>> tasks_;  // Task queue
    std::mutex queue_mutex_;  // Mutex to synchronize access to task queue
    std::condition_variable condition_;  // Condition variable for task availability
    bool stop_;  // Flag to stop all threads
};

#endif // PARALLEL_SCHEDULER_H

