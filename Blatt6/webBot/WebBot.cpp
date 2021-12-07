/**************************************************************   
 * cppShell.cpp  
 * Funktion: Schnittstelle fürs Terminal. Liest eingaben ein, löst Variablen auf und ruft die entsprechnenden Unix Befehle aus 
 * Autor: Dennis Mitkevic, Tim Lock
 *************************************************************/
#include "WebBot.h"


WebBot::WebBot(int queueSize, int threadCount, const string &steuerDatei) : threadCount(threadCount), queue(queueSize), steuerDatei(steuerDatei){
}

void WebBot::initialize(){
    /*
    char *argv[3];
    argv[0] = "testSites_10_2019.txt";
    argv[1] = "--webreq-delay 100 ";
    argv[2] = "--webreq-path download";
    webRequest = new WebRequest(3,argv);
     */
}
void WebBot::reader() {
    fstream file(steuerDatei);
    if(!file){
      cerr << "Datei " << steuerDatei << " kann nicht geoeffnet werden\n";
    }
    string input;
    while (file>> input){
        if(queue.isFull());
            //warten
        queue.addItem((char*)input.c_str());
    }
}
void WebBot::client() {
    int id = threadID++;
    string url;
    fstream f;
    f.open( "fooey.txt", ios::out );
    f << flush;
    f.close();
    while(!queue.isEmpty()){
        stringstream filename;
        queue.delItem(url);
        filename << id << "_" << fileCount++ << "_" << url << ".html";
        cout << filename.str() << endl;
        fstream file;
        file.open(filename.str(), ios::out);
        file << flush;
        if(!file) cerr << "Kann Datei nicht erstellen\n";
        file.close();
        char *argv[3];
        argv[0] = "testSites_10_2019.txt";
        argv[1] = "--webreq-delay 100 ";
        string tmp("--webreq-path ");
        tmp.append(filename.str());
        argv[2] = (char*) tmp.c_str();
        WebRequest webRequest(3,argv);
       // webRequest.download(url,filename.str());
    }
}

void WebBot::run(){
    reader();
    client();
}
