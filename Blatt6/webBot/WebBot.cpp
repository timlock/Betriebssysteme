/**************************************************************   
 * cppShell.cpp  
 * Funktion: Schnittstelle fürs Terminal. Liest eingaben ein, löst Variablen auf und ruft die entsprechnenden Unix Befehle aus 
 * Autor: Dennis Mitkevic, Tim Lock
 *************************************************************/
#include "WebBot.h"



void WebBot::initialize(){
    char *argv[3];
    argv[0] = "testSites_10_2019.txt";
    argv[1] = "--webreq-delay 100 ";
    argv[2] = "-webreq-path";
    WebRequest request(3,argv);
    request.download("https://www.google.de/","download/");
}

