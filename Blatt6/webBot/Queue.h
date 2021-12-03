#pragma once 
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
using namespace std;

class Queue {
private:
    int *buf;
    int capacity;
    long head, tail;
    bool full, empty;
public:
    std::mutex mut;
    std::condition_variable notFull, notEmpty;
public:
    Queue(int capacity);
    ~Queue();
    void addItem(int in);
    void delItem(int *out);
    bool isFull();
    bool isEmpty();
};