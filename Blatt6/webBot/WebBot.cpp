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
WebBot::WebBot(int queueSize, int threadCount, const string &steuerDatei, int delay,bool debug)
: threadCount(threadCount), queue(queueSize,debug), steuerDatei(steuerDatei), delay(delay),  debug(debug){
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
    //tmpDelay.append(to_string(delay));
    argv[1] = (char*)tmpDelay.c_str();
    argv[2] = (char*)"--webreq-path download";
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
      exit(-1);
    }
    string input;
    while (file >> input){ // Liest Zeilenweise die Einträge der SteuerDatei in den String input
        unique_lock<mutex> lock(mut); // Kritischer Abschnitt wird erreicht, Thread erhält den Mutex. kein anderer Thread kann nun in einen kritischen Bereich eintreten
        queue.addItem((char*)input.c_str()); // Link aus der SteuerDatei wird zur Queue hinzugefügt
        notEmpty.notify_all(); // Andere Threads werden informiert, dass in der Queue verfügbare Elemente sind
        if(debug) cout << "notEmpty" << endl;
        if(queue.isFull()){
            if(debug) cout << "Queue ist voll\n";
            notFull.wait(lock, [this](){return !queue.isFull();}); // thread wartet bis die Bedienung wahr wird und gibt in der Zwischenzeit das mutex wieder frei,
        }
        if(file.eof()) readComplete = true; // Letzte Zeile wurde gelesen und Signal wird gesetzt
        lock.unlock();
        //lambda soll verhindern, dass der thread weiterläuft falls er zufällig aufwacht, kann in seltenen Fällen passieren
    }
    cout << "__SteurDatei ist leer__\n";

}
/*
 * Nimmt einen Link aus der Queue und lädt die html runter und speichert sie unter
 * <threadId>_<Zeilennummer des Links in der SteuerDatei>_<link>.html
 */
void WebBot::client() {
    int id = threadID++; // Jeder thread erhält eine ID
    string url;
    int readFiles = 0;
    while(!readComplete || !queue.isEmpty()){ // Läuft solange die Steuerdatei ungelesene Einträge hat oder die Queue noch Einträge hat
        this_thread::sleep_for(chrono::milliseconds(delay)); // thread wartet für delay in millisekunden
        unique_lock<mutex> lock(mut); // Kritischer Bereich wird erreicht und thread erhält das mutex
        if(queue.isEmpty()){
            if(debug)cout << id << " wartet auf neue Einträge" << endl;
            notEmpty.wait(lock, [this](){return !queue.isEmpty() || signalDone;});
            if(signalDone)break; // Thread befindet sich im Deadlock, da es keine URLs mehr gibt und verlässt die Methode
            else continue; // Queue hat neue Einträge und der Thread beginnt die Schleife von vorne
        }
        stringstream ssfilename;
        queue.delItem(url); // Inhalt am Kopfende der Queue wird in url gelesen
        if(debug){
            cout << "CLIENT " << id << " Liest  " << url << endl;
        }
        readFiles++;
        notFull.notify_one(); // Reader wird informiert, dass wieder Platz in der Queue ist
        if(debug) cout << "notFUll" << endl;
        lock.unlock(); // Kritischer Bereich wurde verlassen
        ssfilename << "download/" << id << "_" << fileCount++ << "_" << url << ".html";
        string filename(ssfilename.str());
        removeSlash(filename);
        webRequest->download(url,filename); // Download der html Datei
    }
    cout <<"__Thread_ " <<  id << " hat " << readFiles << " gelesen__" << endl;
    signalDone = true; // Queue und SteuerDatei sind leer, Signal wird gegeben um alle wartenden Threads aus dem Deadlock zu befreien
    notEmpty.notify_all();
}

/*
 * erstellt einen reader thread und mehrere client threads
 */
void WebBot::run(){
    auto start = chrono::steady_clock::now();
    thread producer(&WebBot::reader, this); // Quelle https://stackoverflow.com/questions/10673585/start-thread-with-member-function
    thread consumer[threadCount];
    for(int i = 0; i < threadCount;i++){
        consumer[i] = thread(&WebBot::client, this);
    }
    producer.join(); // main thread wartet auf producer thread, da das Programm nicht  beendet werden darf, solange die anderen threads noch laufen
    for(int i = 0; i < threadCount; i++){
        consumer[i].join(); // main thread wartet auf producer thread
    }
    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in milliseconds: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

}
