#include "cppShell.h"

MiniShell::MiniShell(const char *user, const char *path) : user(user), strPath(path)
{
}

void MiniShell::run()
{
    string input;
    while (1)
    {
        cout << user << "@Linux:~" << strPath << "$ ";
        getline(cin, input);
        if(digest(input) == -1) continue;
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
        if(arg[0] == '$'){
            arg = string(getenv(arg.substr(1,arg.length()-1).c_str()));
        }
        this->inputVec.push_back(arg);
    }
    if (inputVec.size() == 0)
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

    Ähndert den Pfad 
*/

int MiniShell::changeDir(string path)
{
    char *chrPath = realpath(path.c_str(), nullptr);
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
int MiniShell::miniExport(vector<string> &inputVec)
{
    int pos = inputVec[1].find("=");
    string varName = inputVec[1].substr(0,pos);
    string varConent = inputVec[1].substr(pos, inputVec[1].length() - pos);
    setenv(varName.c_str(),varConent.c_str(),0);
    return 0;
}

int MiniShell::miniUnix(vector<string> &inputVec)
{
    const char* cmd = inputVec[0].c_str();
//    inputVec.erase(inputVec.begin());
    //vector<char *> charVec = convertToChar(inputVec);
    vector<char*> charVec;
    for(string arg: inputVec){
        charVec.push_back((char*)arg.c_str()); // konvertiert alle Vectorspalten zu char* 
    }
    charVec.push_back(nullptr);
    int state = 0;
    pid_t pid = fork();
    if (pid == -1)
        return -1;
    if (pid > 0)
    {
        waitpid(pid, &state, 0);
        //return WIFEXITED(state);
    }
    if (pid == 0)
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