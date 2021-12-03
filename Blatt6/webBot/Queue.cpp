#include "Queue.h"


Queue::Queue (int capacity) : capacity(capacity){
    empty = true;
    full = false;
    head = 0;
    tail = 0;
}

Queue::~Queue () { }

void Queue::addItem(int in) {
    buf[tail] = in;
    tail++;
    if (tail == capacity)
        tail = 0;
    if (tail == head)
        full = 1;
    empty = 0;
}

void Queue::delItem (int *out) {
    *out = buf[head];
    head++;
    if (head == capacity)
        head = 0;
    if (head == tail)
        empty = 1;
    full = 0;
}

bool Queue::isFull() { return full; }
bool Queue::isEmpty() { return empty; }