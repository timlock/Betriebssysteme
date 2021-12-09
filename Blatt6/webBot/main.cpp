#include <iostream>
#include "WebBot.h"

//int main(int argc, char** argv) {
  int main(){
    std::cout << "Hello, world!\n";
    WebBot *b = new WebBot(10,10, "testSites_10_2020.txt", 100, true);
    b->initialize();
    b->run();
}
