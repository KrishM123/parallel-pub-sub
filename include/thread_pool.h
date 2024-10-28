#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

class ThreadPool {
public:
    static ThreadPool& getInstance(size_t numThreads = std::thread::hardware_concurrency()); 

    void enqueue(std::function<void()> task);
    
    void waitForAll();
    
    ~ThreadPool();

private:
    explicit ThreadPool(size_t numThreads);

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;    // queue chosen instead of vector for FIFO structure as removal from the front of the structure won't require shifting all elements 
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};
