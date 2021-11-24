#include "cppShell.h"

vector<string> MiniShell::pathify(string path)
{
    int pos = 0;
    vector<string> vecPath;
    while ((pos = path.find("/")) != std::string::npos)
    {
        vecPath.push_back(path.substr(0, pos));
        path.erase(0, pos + 1);
    }
    vecPath.push_back(path);
    for (string i : vecPath)
    {
        cout << i << endl;
    }
    return vecPath;
}

MiniShell::MiniShell(char *user, char *path) : user(user), strPath(path)
{
    vecPath = pathify(strPath);
}

void MiniShell::run()
{
    string input;
    while (input != "-1")
    {
        cout << user << "@Linux:" << strPath;
        cin >> input;
        digest(input);
    }
}

void MiniShell::digest(string input)
{
    vector<string> inputVec;
    string arg;
    stringstream sstream(input);
    int count = 0;
    while (sstream >> arg)
    {
        inputVec.push_back(arg);
    }
    if (inputVec.size() == 0)
        return;
    execute(inputVec);
}

int MiniShell::execute(vector<string> &inputVec)
{
    if (inputVec[0] == "cd")
        changeDir(inputVec);
    if (inputVec[0] == "exit")
        miniExit(inputVec);
    if (inputVec[0] == "showenv")
        showenv(inputVec);
    if (inputVec[0] == "export")
        miniExport(inputVec);


        return 0;
}

void MiniShell::changeDir(vector<string> &inputVec){
    
}
void MiniShell::miniExit(vector<string> &inputVec){

}
void MiniShell::showenv(vector<string> &inputVec){

}
void MiniShell::miniExport(vector<string> &inputVec){

}
/*
int main(int argc, char *argv[])
{
    MiniShell shell(argv[1], argv[2]);
    shell.run();
}
*/
int main(){
    
}