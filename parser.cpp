#include "parser.h"
#include<sstream>
#include<algorithm>
#include<cctype>
using namespace std;
string trim(string s){
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

vector<string> handle_quotes(string command){
    vector<string> tokens;
    string temp=command;
    while(temp.find("\"")!=string::npos){
        int index=temp.find("\"");
        string token=trim(temp.substr(0, index));
        token=trim(token);
        if(token.empty()==false){
            tokens.push_back(token);
        }
        temp=temp.substr(index+1);
    }
    if(temp.empty()==false){
        tokens.push_back(temp);
    }
    vector<string> ans;
    for(int i=0;i<tokens.size();i++){
        if(i%2==1){
            ans.push_back(tokens[i]);
            continue;
        }
        vector<string> temp=split(tokens[i], ' ');
        if(temp.empty()==false){
            for(auto s: temp){
                ans.push_back(s);
            }
        }
    }
    return ans;

}

vector<vector<string>> parseCommands(string &input){
    vector<vector<string>> parsed;
    vector<string> commands=split(input, ';');
    for(auto &command: commands){
        vector<string> tokens;
        if(command.find("\"")!=string::npos){
            tokens=handle_quotes(command);
        }
        else{
            tokens=split(command, ' ');
        }
        if(!tokens.empty()){
            parsed.push_back(tokens);
        }
    }
    return parsed;

}