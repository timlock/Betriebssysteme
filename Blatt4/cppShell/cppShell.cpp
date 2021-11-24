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
        if(input == "-1") miniExit();
        if (execute(this->inputVec) != 0)
            cout << endl
                 << "Fehler beim ausführen" << endl;
                 inputVec.clear();
    }
    cout << "Beende mini shell" << endl;
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
        if(arg[0] == '$'){ // Prüft auf Umgebungsvariable
            arg = string(getenv(arg.substr(1,arg.length()-1).c_str())); // Umgebungsvariable wird aufgelöst
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
            inputVec[0] = "printenv";
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
int MiniShell::showenv(vector<string> &inputVec)
{
    return 0;
}

/*
 * @param inputVec vector<string> & Vector mit allen Eingaben
 *
 * Setzt eine neue Umgebungsvariable
 */
int MiniShell::miniExport(vector<string> &inputVec)
{
    if(inputVec[1].find("=") == string::npos){
        cout << "Fehler beim setzen der Variable" << endl;
        return -1;
    }
    int pos = inputVec[1].find("=");
    string varName = inputVec[1].substr(0,pos);
    string varConent = inputVec[1].substr(pos, inputVec[1].length() - pos);
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
    for(string arg: inputVec){
        charVec.push_back((char*)arg.c_str()); // konvertiert alle Vectorspalten zu char* 
    }
    charVec.push_back(nullptr); // nullptr markiert das Ende von einem C array
    int state = 0;
    pid_t pid = fork(); // es wird ein Kind Prozess erstellt
    if (pid == -1) // Fehler beim Fork
        return -1;
    if (pid > 0) // Process ID des Parent Process hat immer einen Wert größer 0
    {
        waitpid(pid, &state, 0); // Process wartet auf Child
        //return WIFEXITED(state);
    }
    if (pid == 0) // Child Process ist immer 0
    {
        return execvp(cmd, &charVec[0]); //exec -v- es wird ein Vector übergeben -p- der Befehl wird in $PATH gesucht, man muss den Pfad des Befehls deshalb nicht übergeben
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