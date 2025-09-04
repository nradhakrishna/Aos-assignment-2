#include "foreground.h"
#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
// custom strdup
pid_t fg_pid=-1;


void int_handler(int signal){
    if(fg_pid>0){
        kill(fg_pid, SIGINT);

    }
}
void stop_handler(int signal){
    if(fg_pid>0){
        kill(fg_pid, SIGTSTP);
    }
}
char* my_strdup_fg(const string& s){
    int len = s.size() + 1;
    char* dup = new char[len];
    for(int i = 0; i < s.size(); i++) dup[i] = s[i];
    dup[len-1] = '\0';
    return dup;
}

// convert vector<string> to char*[]
char** make_char_pointer_array_fg(const vector<string>& command){
    char** argList = new char*[command.size() + 1];
    for (size_t i = 0; i < command.size(); i++){
        argList[i] = my_strdup_fg(command[i]);
    }
    argList[command.size()] = nullptr;
    return argList;
}

void free_char_array_fg(char** args, int n){
    for(int i = 0; i < n; i++) delete[] args[i];
    delete[] args;
}

void run_foreground(vector<string> command){
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
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        char** argList = make_char_pointer_array_fg(command);
        if(execvp(argList[0], argList) < 0){
            perror("execution failed");
            free_char_array_fg(argList, command.size());
            exit(1);
        }
    } else {
        fg_pid=child;
        
        cout << "foreground process pid " << child << " started" << endl;
        int flag;
        waitpid(child, &flag, WUNTRACED);
        if(WIFEXITED(flag)){
            cout<< "process with pid: "<< child<<", exited with status: "<< WIFEXITED(flag)<< endl;
        }
        fg_pid=-1;

    }
}
