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
using namespace std;

class WebBot {
Queue queue;

public:
WebBot() = default;
~WebBot() = default;
void initialize();




};