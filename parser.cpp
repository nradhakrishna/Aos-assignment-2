#include "parser.h"
#include<sstream>
#include<algorithm>
#include<cctype>
using namespace std;
string trim(string &s){
   size_t start=s.find_first_not_of(" \t");//finds the first index where it doesnot match with any of the given set of chars
   size_t end=s.find_last_not_of(" \t");// same as above but with the last index
   if(start==string::npos)
   {
    return "";
   } 
   return s.substr(start, end-start+1);
}

vector<string> split(string &s, char delimiter){
    vector<string> tokens;
    string token;
    stringstream ss(s);
    while(getline(ss, token, delimiter)){// getline(stream, destination, delimiter)
        token=trim(token);
        if(token.empty()==false){
            tokens.push_back(token);
        }
    }
    return tokens;
}


vector<vector<string>> parseCommands(string &input){
    vector<vector<string>> parsed;
    vector<string> commands=split(input, ';');
    for(auto &command: commands){
        vector<string> tokens=split(command, ' ');
        if(!tokens.empty()){
            parsed.push_back(tokens);
        }
    }
    return parsed;

}