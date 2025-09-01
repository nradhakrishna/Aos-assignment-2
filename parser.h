#ifndef PARSER_H
#define PARSER_H
#include<string>
#include<vector>
using namespace std;

string trim(string s);
vector<string> split(string &s, char delimiter);
vector<vector<string>> parseCommands(string &input);
#endif