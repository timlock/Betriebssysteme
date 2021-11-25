#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

using namespace std;


class MiniShell{
    public:
    vector<string> vecPath;
    string strPath;
    string user;
    vector<string> inputVec;
    int status = 0;

    MiniShell(const char* user, const char* path);
    void run();
    int digest(string input);
    int execute(vector<string> &inputVec);
    int changeDir(string path);
    void miniExit ();
    int showenv (vector<string> &inputVec);
    int miniExport(vector<string> &inputVec);
    int miniUnix(vector<string> &inputVec);
};