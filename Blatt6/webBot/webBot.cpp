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
#include "web_request/web_request.cpp"
#include "Queue.h"
#include <iostream>
using namespace std;

#define QUEUESIZE = 20;




void initialize(){
    Queue queue();

}


int main(int argc, char *argv[]){
    initialize();
    
    return 0;
}