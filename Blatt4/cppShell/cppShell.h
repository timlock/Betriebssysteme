#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


class MiniShell{
    public:
    vector<string> vecPath;
    string strPath;
    string user;

    MiniShell(char* user, char* path);
    vector<string> pathify(string path);
    void run();

};