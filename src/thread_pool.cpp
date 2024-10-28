#include "thread_pool.h"

ThreadPool& ThreadPool::getInstance(size_t numThreads) {
    static ThreadPool instance(numThreads);
    return instance;
}

ThreadPool::ThreadPool(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {   // emplace_back used here to avoid unncessary temp variables and copies
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);  // scope of lock limited. Will be unlocked after end of scope because of unique_lock
                    condition.wait(lock, [this] {   // waits until there is a task; releases lock while waiting; updates everytime condition is notified
                        return stop || !tasks.empty();
                    });

                    if (stop && tasks.empty()) {    // makes sure all threads exit gracefully and no pending work is left
                        return;
                    }

                    task = std::move(tasks.front());    // vector.move avoids unnecessary copying. Empty version of task left on queue
                    tasks.pop();    // empty version removed
                }
                
                try {
                    task();
                } catch (const std::exception& e) {
                    std::cerr << "Error executing task: " << e.what() << std::endl;
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);  // scoped access of mutex to change stop
        stop = true;
    }
    condition.notify_all();     // mutex not needed here as threads will get mutex themselves
    // joinable threads are threads that are not yet finished and we care about them finishing (all)
    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();  // waits for thread to finish; clean shutdown
        }
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (stop) {
            throw std::runtime_error("Cannot enqueue on stopped ThreadPool");
        }
        tasks.emplace(std::move(task));     // adds task function onto tasks; moves ownership into tasks without making a copy
    }
    condition.notify_one();
}

void ThreadPool::waitForAll() {     // waits until all tasks in queue are completed
    std::unique_lock<std::mutex> lock(queueMutex);      // required to check whether queue is empty
    condition.wait(lock, [this] {
        return tasks.empty();
    });
}