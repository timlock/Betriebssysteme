#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "web_request/web_request.cpp"
#include "Queue.h"
#include <iostream>
using namespace std;

class webBot {
Queue queue;

public:
webBot();
~webBot();
void initialize();




};