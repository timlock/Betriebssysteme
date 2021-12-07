#include "Queue.h"


Queue::Queue (int capacity) : capacity(capacity), buf(capacity, ""){
    empty = true;
    full = false;
    head = 0;
    tail = 0;
}


void Queue::addItem(const string &in) {
    unique_lock<mutex> lock(mut);
    notFull.wait(lock, [this](){return !this->empty;}); // thread wartet bis die Bedienung wahr wird und gibt das mutex wieder frei,
    // lambda soll verhindern, dass der thread weiterläuft, falls der thread ausversehen aufwacht
    buf[tail] = in;
    tail = (tail + 1) % capacity;
//    if (tail == capacity)
//        tail = 0;
    if (tail == head){
        full = true;
    }
    empty = false;
    notEmpty.notify_all();
    mut.unlock();
}

void Queue::delItem (string & out) {
    unique_lock<mutex> lock(mut);
    notEmpty.wait(lock, [this](){return !this->full;});
    out = buf[head];
    head = (head + 1) % capacity;
//    if (head == capacity)
//        head = 0;
    if (head == tail)
        empty = true;
    full = false;
    notFull.notify_one();
    mut.unlock();
}

bool Queue::isFull() { return full; }
bool Queue::isEmpty() { return empty; }