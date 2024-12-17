// parallel_scheduler.cpp

#include "parallel_scheduler.h"
#include <iostream>

parallel_scheduler::parallel_scheduler(size_t capacity)
    : capacity_(capacity), stop_(false) {
    // Create the threads
    for (size_t i = 0; i < capacity_; ++i) {
        workers_.emplace_back(&parallel_scheduler::worker, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        stop_ = true;  // Set the stop flag
    }
    condition_.notify_all();  // Notify all threads to stop

    // Join all threads
    for (auto &worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void parallel_scheduler::run(std::function<void()> func) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        tasks_.push(func);  // Add the task to the queue
    }
    condition_.notify_one();  // Notify one thread to work on the task
}

void parallel_scheduler::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this]() { return !tasks_.empty() || stop_; });

            if (stop_ && tasks_.empty()) {
                return;  // Exit if stop is true and there are no more tasks
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();  // Execute the task
    }
}

