#include "background.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <vector>
using namespace std;

// custom strdup
char* my_strdup(const string& s){
    int len = s.size() + 1;
    char* dup = new char[len];
    for(int i = 0; i < s.size(); i++) dup[i] = s[i];
    dup[len-1] = '\0';
    return dup;
}

// convert vector<string> to char*[]
char** make_char_pointer_array(const vector<string>& command){
    char** argList = new char*[command.size() + 1];
    for (size_t i = 0; i < command.size(); i++){
        argList[i] = my_strdup(command[i]);
    }
    argList[command.size()] = nullptr;
    return argList;
}

void free_char_array(char** args, int n){
    for(int i = 0; i < n; i++) delete[] args[i];
    delete[] args;
}

void run_background(vector<string> command){
    if(command.empty()){
        cerr << "No command given\n";
        return;
    }
    cout.flush();
    // for(auto &c : command) cout << "[" << c << "] ";
    // cout << endl;
    pid_t child = fork();
    if(child < 0){
        perror("fork failed");
        return;
    } 
    else if(child == 0){
        char** argList = make_char_pointer_array(command);
        if(execvp(argList[0], argList) < 0){
            perror("execution failed");
            free_char_array(argList, command.size());
            exit(1);
        }
    } else {
        // waitpid(child, NULL, 0);
        cout << "background process pid " << child << " started" << endl;

    }
}
