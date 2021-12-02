/**************************************************************   
 * cppShell.cpp  
 * Funktion: Schnittstelle fürs Terminal. Liest eingaben ein, löst Variablen auf und ruft die entsprechnenden Unix Befehle aus 
 * Autor: Dennis Mitkevic, Tim Lock
 *************************************************************/

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#define QUEUESIZE = 20;

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

void initialize(){
    Queue queue();
    thread(initialize, );

}


int main(int argc, char *argv[]){
    initialize();
    return 0;
}