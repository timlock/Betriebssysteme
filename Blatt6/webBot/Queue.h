#pragma once
#include <iostream>
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
    bool full, empty, debug;

public:
    Queue(int capacity,bool debug);
    Queue() = default;
    ~Queue() = default;
    void addItem(const string &in);
    void delItem(string & out);
    bool isFull();
    bool isEmpty();
};