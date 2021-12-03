#include "Queue.h"


Queue::Queue () {
    empty = true;
    full = false;
    head = 0;
    tail = 0;
}

Queue::~Queue () { }

void Queue::addItem(int in) {
    buf[tail] = in;
    tail++;
    if (tail == 20)
        tail = 0;
    if (tail == head)
        full = 1;
    empty = 0;
}

void Queue::delItem (int *out) {
    *out = buf[head];
    head++;
    if (head == 20)
        head = 0;
    if (head == tail)
        empty = 1;
    full = 0;
}