#include "Queue.h"


Queue::Queue (int capacity) : capacity(capacity), buf(capacity, ""){
    empty = true;
    full = false;
    head = 0;
    tail = 0;
}


void Queue::addItem(const string &in) {
    buf[tail] = in;
    tail = (tail + 1) % capacity;
//    if (tail == capacity)
//        tail = 0;
    if (tail == head)
        full = true;
    empty = false;
}

void Queue::delItem (string & out) {
    out = buf[head];
    head = (head + 1) % capacity;
//    if (head == capacity)
//        head = 0;
    if (head == tail)
        empty = true;
    full = false;
}

bool Queue::isFull() { return full; }
bool Queue::isEmpty() { return empty; }