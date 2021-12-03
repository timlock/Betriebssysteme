#pragma once 
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

class Queue {
private:
    int buf[20];
    long head, tail;
    bool full, empty;
public:
    std::mutex mut;
    std::condition_variable notFull, notEmpty;
public:
    Queue();
    ~Queue();
    void addItem(int in);
    void delItem(int *out);
    bool isFull() { return full; }
    bool isEmpty() { return empty; }
};