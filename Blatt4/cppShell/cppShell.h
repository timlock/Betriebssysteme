#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;


class MiniShell{
    public:
    vector<string> vecPath;
    string strPath;
    string user;
    int status = 0;

    MiniShell(char* user, char* path);
    vector<string> pathify(string path);
    void run();
    void digest(string input);
    int execute(vector<string> &inputVec);
    void changeDir(vector<string> &inputVec);
    void miniExit (vector<string> &inputVec);
    void showenv (vector<string> &inputVec);
    void miniExport(vector<string> &inputVec);
};