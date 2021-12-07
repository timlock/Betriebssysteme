#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
using namespace std;

class Queue {
private:
    vector<string> buf ;
    int capacity;
    int delay;
    long head, tail;
    bool full, empty;
public:
    std::mutex mut;
    std::condition_variable notFull, notEmpty,done;
public:
    Queue(int capacity);
    Queue() = default;
    ~Queue() = default;
    void addItem(const string &in);
    void delItem(string & out);
    bool isFull();
    bool isEmpty();
};