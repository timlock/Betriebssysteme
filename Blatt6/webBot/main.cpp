#include <iostream>
#include "WebBot.h"

//int main(int argc, char** argv) {
  int main(){
    std::cout << "Hello, world!\n";
    WebBot *b = new WebBot(100,1, "testSites_10_2020.txt");
    b->initialize();
    b->run();
}
