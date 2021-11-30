/**************************************************************   
 * cppShell.cpp  
 * Funktion: Schnittstelle fürs Terminal. Liest eingaben ein, löst Variablen auf und ruft die entsprechnenden Unix Befehle aus 
 * Autor: Dennis Mitkevic, Tim Lock
 *************************************************************/




#include "cppShell.h"

MiniShell::MiniShell(const char *user, const char *path) : user(user), strPath(path)
{
}

/*
 *Liest permanent die Nutzereingabe und ruft die entsprechenden Methoden auf
 */
void MiniShell::run()
{
    string input;
    while (1)
    {
        cout << user << "@Linux:~" << strPath << "$ ";
        getline(cin, input); //Liest komplette Eingabe in einen String
        if(digest(input) == -1) continue; //Bei einer leeren Eingabe wird der aktuelle Schleifendurchgang abgebrochen
        if (execute(this->inputVec) != 0)
            cout << endl
                 << "Fehler beim ausführen" << endl;
                 inputVec.clear();
    }
}
/*
    @param string input Eingabe des Benutzers

    Speichert die Eingabe wortweise die instanzvariable vector<string> inputVec
*/

int MiniShell::digest(string input)
{
    string arg;
    stringstream sstream(input);
    int count = 0;
    while (sstream >> arg)
    {
        char* chrPtr = nullptr;
        if(arg[0] == '$'){ // Prüft auf Umgebungsvariable
            //arg = string(getenv(arg.substr(1,arg.length()-1).c_str())); // Umgebungsvariable wird aufgelöst
            chrPtr = getenv(arg.c_str() + 1);
            if(chrPtr) arg = string(chrPtr);
            else arg= string("");
        }
        this->inputVec.push_back(arg);
    }
    if (inputVec.size() == 0) // leere Eingabe wird abgefangen
        return -1;
        else return 0;
}

/*
    @param inputVec vector<string> & referenz auf den Vector mit den Eingaben

    überprüft anhand der ersten Vector Stelle welcher Befehl ausgeführt werden soll und ruft die entsprechende Methode auf

    Wenn der Befehl nicht ordnungsgemäß aufgerufen werden konnte wird -1 zurückgegeben, ansonsten 0
*/

int MiniShell::execute(vector<string> &inputVec)
{
    if (inputVec[0] == "cd" && inputVec.size() == 2)
        return changeDir(inputVec[1]);
    if (inputVec[0] == "exit")
        miniExit();
        if (inputVec[0] == "showenv"){
            if(inputVec.size() > 1) showenv(inputVec[1]);
            else showenv(string(""));
        }
    if (inputVec[0] == "export" && inputVec.size() > 1)
        return miniExport(inputVec);

    return miniUnix(inputVec);
}

/*
    @param path string Zielpfad

    Ändert den Pfad
*/

int MiniShell::changeDir(string path)
{
    char *chrPath = realpath(path.c_str(), nullptr); // Relativer Pfad wird aufgelöst
    if (chdir(chrPath) == -1)
    {
        cout << "Pfad existiert nicht" << endl;
        return -1;
    }
    strPath = string(chrPath);
    return 0;
}
void MiniShell::miniExit()
{
    exit(0);
}
int MiniShell::showenv(string var)
{

    extern char **environ;
    for(size_t i = 0; environ + i != nullptr;i++){
        cout << *(environ +i) << endl;
    }

    return 0;
}

/*
 * @param inputVec vector<string> & Vector mit allen Eingaben
 *
 * Setzt eine neue Umgebungsvariable
 */
int MiniShell::miniExport(vector<string> &inputVec)
{
    size_t pos = inputVec[1].find("=");
    if(pos == string::npos){
        cout << "Fehler beim setzen der Variable" << endl;
        return -1;
    }
    string varName = inputVec[1].substr(0,pos); // substring vom Anfang bis "="
    string varConent = inputVec[1].substr(2 + pos, inputVec[1].length() - pos -3); // substring von "="  bis zum Ende
    setenv(varName.c_str(),varConent.c_str(),0); // Variable wird gestzt
    return 0;
}
/*
 * @param inputVec vector<string> & Vector mit allen Eingaben
 *
 * führt einen Unix Befehl aus
 */

int MiniShell::miniUnix(vector<string> &inputVec)
{
    const char* cmd = inputVec[0].c_str();
    vector<char*> charVec;
    vector<char*> charVec2;
    bool containsPipe = false;//Eingabe enthält eine Pipe mit zwei Befehlen
    for(int i = 0; i < inputVec.size(); i++){
        if(inputVec[i] == "|") containsPipe = true;
        if(containsPipe){
            charVec2.push_back((char*) inputVec[i].c_str()); //Zweiter Befehl wird seperat gespeichert 
        }
        else charVec.push_back((char*) inputVec[i].c_str());
    }
    charVec.push_back(nullptr); // nullptr markiert das Ende von einem C array
    if(containsPipe) charVec2.push_back(nullptr);
    int state = 0;
    int fd[2]; //Dateideskriptor
    if(pipe(fd) == -1) return -1; 
    pid_t pid = fork(); // es wird ein Kind Prozess erstellt
    pid_t pid2 = 0; //wird benögtigt falls eine Pipe erkannt wurde
    int state2 = 0;
    if (pid == -1) // Fehler beim Fork
        return -1;
    if (pid == 0) // Child Process ist immer 0
    {
        if(containsPipe) {
            dup2(fd[1], STDOUT_FILENO); // das Ergebnis von exec wird nicht mehr über STDOUT ausgegeben sondern an fd[1] an den anderen Child Process weitergeleitet
        }
        exit (execvp(cmd, &charVec[0])); //exec -v- es wird ein Vector übergeben -p- der Befehl wird in $PATH gesucht, man muss den Pfad des Befehls deshalb nicht übergeben
    }
    if (pid > 0) // Process ID des Parent Process hat immer einen Wert größer 0
    {
        waitpid(pid, &state, 0); // Process wartet auf Child
        //return WIFEXITED(state);
        return state;
    }
    return 0;
}



int main()
{
    char *username = getenv("USER"); //Liefert den Wert der Linux Umgebungsvariable $USER
    char *path = nullptr;
    path = getcwd(nullptr, PATH_MAX); // gibt den aktuellen Pfadnamen zurück, MAX_PATH ist die maximale Bitlänge die ein Pfad haben kann
    MiniShell shell(username, path);
    shell.run();
    return 0;
}