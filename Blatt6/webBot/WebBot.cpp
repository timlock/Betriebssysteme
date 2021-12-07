/**************************************************************   
 * cppShell.cpp  
 * Funktion: Schnittstelle fürs Terminal. Liest eingaben ein, löst Variablen auf und ruft die entsprechnenden Unix Befehle aus 
 * Autor: Dennis Mitkevic, Tim Lock
 *************************************************************/
#include "WebBot.h"

/*
 * Konstruktor
 * @param queueSize int Größe der Queue
 * @param threadCount int Anzahl der Consumer/Client threads
 * @param steuerDatei string name der Datei mit den Links der Webseiten die heruntergeladen werden sollen
 *
 */
WebBot::WebBot(int queueSize, int threadCount, const string &steuerDatei, int delay)
: threadCount(threadCount), queue(queueSize), steuerDatei(steuerDatei), delay(delay){
}

/*
 * initialisiert die Klasse WebRequest
 * Parameter stammen aus der BeispielDatei von webRequest
 * 1) Name der steuerDate
 * 2) Länge des Delays in ms
 * 3) ?
 */
void WebBot::initialize(){
    char *argv[3];
    argv[0] = (char*)steuerDatei.c_str();
    string tmpDelay("--webreq-delay");
    tmpDelay.append(to_string(delay));
    argv[1] = (char*)tmpDelay.c_str();
    argv[2] = "--webreq-path download";
    webRequest = new WebRequest(3,argv);

}
/*
 * Entfernt alle '/' aus dem Dateinamen, da in Linux '/' nicht bestandteil eines Dateinamens sein darf
 * @param input string
 */
void WebBot::removeSlash(string &input){
    replace( input.begin(), input.end(), '/', '\\');
}

/*
 * Liest Zeilenweise die Links der SteuerDatei in die Queue
 */
void WebBot::reader() {
    fstream file(steuerDatei);
    if(!file){
      cerr << "Datei " << steuerDatei << " kann nicht geoeffnet werden\n";
    }
    string input;
    while (file>> input){
        this_thread::sleep_for(chrono::milliseconds(delay)); // thread wartet für delay in millisekunden
        if(queue.isFull());

        queue.addItem((char*)input.c_str());
    }
}
/*
 * Nimmt einen Link aus der Queue und lädt die html runter und speichert sie unter
 * <threadId>_<Zeilennummer des Links in der SteuerDatei>_<link>.html
 */
void WebBot::client() {
    int id = threadID++;
    string url;
    while(!queue.isEmpty()){
        this_thread::sleep_for(chrono::milliseconds(delay)); // thread wartet für delay in millisekunden
        stringstream ssfilename;
        queue.delItem(url);
        ssfilename << id << "_" << fileCount++ << "_" << url << ".html";
        string filename(ssfilename.str());
        removeSlash(filename);
        cout << filename << endl;
        webRequest->download(url,filename);
    }
}

/*
 * erstellt einen reader thread und mehrere client threads
 */
void WebBot::run(){
    thread producer(&WebBot::reader, this); // Quelle https://stackoverflow.com/questions/10673585/start-thread-with-member-function
    thread consumer[threadCount];
    for(int i = 0; i < threadCount;i++){
        consumer[i] = thread(&WebBot::client, this);
    }

}
