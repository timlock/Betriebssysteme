#include "cppShell.h"


 vector<string> MiniShell::pathify(string path){
        int pos = 0;
        vector<string> vecPath;
        while ((pos = path.find("/")) != std::string::npos) {
            vecPath.push_back(path.substr(0, pos));
            path.erase(0, pos + 1);
        }       
        vecPath.push_back(path);
        for(string i: vecPath){
            cout << i << endl;
        }
        return vecPath;
    }

MiniShell::MiniShell(char* user, char* path):user(user), strPath(path){
        vecPath = pathify(strPath);
    }


    void MiniShell::run (){
        string input;
        while(input != "-1"){
            cout  << user << "@Linux:" << strPath;
            cin >> input;
        }

    }



int main(int argc, char *argv[])
{
    MiniShell shell(argv[1],argv[2]);
    shell.run();
}