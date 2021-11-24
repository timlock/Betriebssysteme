//
// Created by linkurze@id.hsos.de on 18.11.21.
//

#ifndef PRAKTIKUM_4_MAIN_H
#define PRAKTIKUM_4_MAIN_H
void printPrompt();
string readInput();
string removeSpaces(string line);
int testForSetVar(string word);
string* vectorToString(vector<string> v, int start);
vector<string> separate(string line);
vector<string> interpretVars(vector<string> words);

#endif //PRAKTIKUM_4_MAIN_H
