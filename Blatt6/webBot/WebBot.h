#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "web_request/web_request.cpp"
#include "Queue.h"
#include <iostream>
#include <sstream>
using namespace std;

class WebBot {
Queue queue;
//WebRequest *webRequest = nullptr;
int threadCount;
int threadID = 1;
int fileCount = 0;
const string steuerDatei;

public:
WebBot() = delete; // es darf nur der überladene Konstruktor verwendet werden
WebBot(int queuesize, int threadCount, const string &steuerDatei);
~WebBot() = default;
void initialize();
void reader();
void client();
void run();




};