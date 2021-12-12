#include <iostream>
#include "WebBot.h"

int main(int argc, char** argv) {
   /*
  int main(){
  int queueSize = 5;
  int threadCount = 20;
  int delay = 100;
  */
    int queueSize = stoi(string(argv[1]));
    int threadCount = stoi(string(argv[2]));
    int delay = stoi(string(argv[3]));
  WebBot *b = new WebBot(queueSize,threadCount, "testSites_10_2020.txt", delay, true);
//    WebBot *b = new WebBot(10,20, "testSites_10_2020.txt", 100, true);
    b->initialize();
    b->run();
}
