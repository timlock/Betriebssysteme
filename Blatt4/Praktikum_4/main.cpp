/**************************************************************
 * main.cpp
 * Funktion: Ist die main-Klasse des Programms, welche die Mini-Shell aufruft.
 * Autoren: Mihaela Angelove, Linus Kurze, HSOS
 * Historie:
 *   2021/11/18: Programm & printPrompt erstellt
 *   2021/11/21: Verarbeitung der Eingaben begonnen umzusetzen
 *   2021/11/22: Struktur für Verarbeitung geschaffen, funktionen umgesetzt
 * To Do:
 * - ggf. Fehler abfangen und referenzen/pointer verwenden
 *************************************************************/

#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

using namespace std;

#include "main.h"

#define PATH_SIZE 4096
char path[PATH_SIZE];
string username;

int main() {
    // init
    username = getenv("USER");
    getcwd(path, PATH_SIZE);

    while (true) {
        printPrompt();
        vector<string> interpretedInput = interpretVars(separate(readInput()));

        if (interpretedInput.size() > 0) {
            if (interpretedInput[0] == "export" && interpretedInput.size() > 1) {
                for (int i = 1; i < interpretedInput.size(); i++) {
                    int pos = testForSetVar(interpretedInput[i]);
                    if (pos != -1) {
                        setenv(interpretedInput[0].substr(0, pos).c_str(),
                               interpretedInput[0].substr(pos + 2, interpretedInput[0].length() - pos - 3).c_str(),
                               1); // Gleich und Anfuehrungszeichen abschneiden
                    }
                }
            } else if (interpretedInput[0] == "cd" && interpretedInput.size() == 2) {
                chdir(interpretedInput[1].c_str());
                getcwd(path, PATH_SIZE);
            } else if (interpretedInput[0] == "showenv") {
                for (int i = 1; i < interpretedInput.size(); i++) {
                    cout << getenv(interpretedInput[i].c_str()) << endl;
                }
            } else if (interpretedInput[0] == "exit") {
                break;
            } else { // unix command
                int pid = fork();
                while (pid < 0) {
                    pid = fork();
                }
                if (pid != 0) {
                    //parent
                    int status;
                    pid = waitpid(pid, &status, 0);
                } else {
                    //child

                    //cast to cstring vector
                    std::vector<char*> cstrings;
                    cstrings.reserve(interpretedInput.size());

                    for(size_t i = 0; i < interpretedInput.size(); ++i) {
                        cstrings.push_back(const_cast<char *>(interpretedInput[i].c_str()));
                    }
                    cstrings.push_back(nullptr);

                    cout << execvp(interpretedInput[0].c_str(),&cstrings[0]) << endl;
                }
            }
        }
    }
    return 0;
}

string* vectorToString(vector<string> v, int start) {
    string* array = new string[v.size() - start];
    for (int i = start; i < v.size(); i++) {
        array[i - start] = v[i].c_str();
    }
    return array;
}

void printPrompt() {
    cout << username << ":~" << path << "$ ";
}

string readInput() {
    string input;
    getline(cin, input);
    return removeSpaces(input);
}

int testForSetVar(string word) {
    int pos = word.find("="); // kein Treffer = returnt -1
    if (word[pos + 1] == '"' && word[word.length() - 1] == '"') {
        return pos;
    }
    return -1;
}

string removeSpaces(string line) {
    for (int i = 1; i < line.length(); i++) {
        if (line[i] == ' ' && line[i - 1] == ' ') {
            line.erase(i, 1);
            i--;
        }
    }
    if (line[0] == ' ') {
        line.erase(0, 1);
    }
    if (line[line.length() - 1] == ' ') {
        line.erase(line.length() - 1, 1);
    }
    return line;
}

vector<string> separate(string line) {
    vector<string> words;
    int start = 0;
    while (line.length() > start) {
        int end = line.find(' ', start);
        if (end == -1) {
            end = line.length();
        }
        words.push_back(line.substr(start, end - start));
        start += end + 1;
    }
    return words;
}

vector<string> interpretVars(vector<string> words) {
    for (int i = 1;
         i < words.size(); i++) { // Erstes wort nicht interpretieren, da dort z.B. Komando oder  Variablenname steht
        if (words[i][0] == '$') {
            int length = words[i].length() - 1;
            int pos = words[i].find("/");
            if (pos != -1) {
                length = pos;
            }
            string result = "";
            if (getenv((words[i].substr(1, length)).c_str())) {
                result = getenv((words[i].substr(1, length)).c_str());
            }
            words[i] = result;
        }
    }
    return words;
}